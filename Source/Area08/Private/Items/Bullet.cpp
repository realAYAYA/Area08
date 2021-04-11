// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"// 用于播放特效粒子动画
#include "PhysicalMaterials/PhysicalMaterial.h"// 用于判断物理材质
#include "Net/UnrealNetwork.h"

#include "Area08/Area08.h"
#include "NumCalculation/Area08DamageType.h"

ABullet::ABullet() {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetGenerateOverlapEvents(true);//打开碰撞检测

	RootComponent = Mesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->InitialSpeed = 3000.f;
	MovementComp->MaxSpeed = 3000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = true;

	Damage = 20.0f;

	DamageType=CreateDefaultSubobject<UArea08DamageType>(TEXT("DamageType" ));

	// Network
	// --------------------------------
	SetReplicates(true);
	SetReplicateMovement(true);
	NetUpdateFrequency=66;
	MinNetUpdateFrequency=33;

}

void ABullet::BeginPlay() {
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnHit);//设置事件触发委托

	SetLifeSpan(8.0f);//设置自我销毁时间,在BeginPlay里可以，在构造函数里调用会报错
}

void ABullet::OnHit(UPrimitiveComponent* OverlappedComponent,
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Hit)
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;// 只有服务器端的子弹命中才可以造成伤害，客户端则不允许
	}
	if (HitActor) {
		EPhysicalSurface tempSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		SetDamageType(tempSurfaceType);// 设置子弹的伤害部位
		
		/* 播放特效、音效**/
		PlayHitEffects(tempSurfaceType,Hit.ImpactPoint,Hit.ImpactNormal.Rotation());
		if(GetLocalRole()==ROLE_Authority)// 子弹命中特效同步到客户端
		{
			HitScanTrace.HitPoint=Hit.ImpactPoint;
			HitScanTrace.HitRotation=Hit.ImpactNormal;
		}
		
		// 服务器判中才会造成伤害，且如果打的材质确实属于可被伤害的类型，才会造成伤害
		if(HasAuthority() && DamageType->HitRegion!=DamageRegion::None)
		{
			// 这里做输出测试
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Bullet::Hit.")), false);
			UGameplayStatics::ApplyPointDamage(HitActor, Damage,
				this->GetActorForwardVector(), Hit, this->GetOwner()->GetInstigatorController(), this, BPDamageType);
		}
		
		if(true)// 这里判断下弹种来决定是否销毁还是过穿
		{
			this->Destroy();
		}
	}
}

void ABullet::PlayHitEffects(EPhysicalSurface tempSurfaceType,const FVector& HitPoint,const FRotator& HitRotation)
{
	SelectEffect=HitFleshEffect;
	
	if(SelectEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),SelectEffect,HitPoint,HitRotation);
	}
}

void ABullet::SetDamageType(const EPhysicalSurface& mySurfaceType)
{
	switch (mySurfaceType)
	{
	case MS_HEAD:
		DamageType->SetHitRegion(DamageRegion::Head);
		break;
	case MS_BODY:
		DamageType->SetHitRegion(DamageRegion::Body);
		break;
	case MS_LARM:
		DamageType->SetHitRegion(DamageRegion::LArm);
		break;
	case MS_RARM:
		DamageType->SetHitRegion(DamageRegion::RArm);
		break;
	case MS_LLEG:
		DamageType->SetHitRegion(DamageRegion::LLeg);
		break;
	case MS_RLEG:
		DamageType->SetHitRegion(DamageRegion::RLeg);
		break;
	default:
		break;
	}
}

void ABullet::OnRep_Hit()
{
	PlayHitEffects(SurfaceType1,HitScanTrace.HitPoint,HitScanTrace.HitRotation.Rotation());
}

void ABullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);	
	DOREPLIFETIME_CONDITION(ABullet,HitScanTrace,COND_SkipOwner);
}
