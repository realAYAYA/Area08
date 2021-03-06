// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"// 用于判断物理材质

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

	DamageType=CreateDefaultSubobject<UArea08DamageType>(TEXT("DamageType"));
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
	if (HitActor) {
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		switch (SurfaceType)
		{
		case MS_HEAD:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Head shoot.")), false);
			DamageType->SetHitRegion(DamageRegion::Head);
			break;
		case MS_BODY:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Body shoot.")), false);
			DamageType->SetHitRegion(DamageRegion::Body);
			break;
		case MS_LARM:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("LeftArm shoot.")), false);
			DamageType->SetHitRegion(DamageRegion::LArm);
			break;
		case MS_RARM:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("RightArm shoot.")), false);
			DamageType->SetHitRegion(DamageRegion::RArm);
			break;
		case MS_LLEG:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("LegLeg shoot.")), false);
			DamageType->SetHitRegion(DamageRegion::LLeg);
			break;
		case MS_RLEG:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("RightLeg shoot.")), false);
			DamageType->SetHitRegion(DamageRegion::RLeg);
			break;
		default:
			break;
		}
		if(DamageType->HitRegion!=DamageRegion::None)// 如果打的材质确实属于可被伤害的类型，才会造成伤害
		{
			UGameplayStatics::ApplyPointDamage(HitActor, Damage,
                this->GetActorForwardVector(), Hit, this->GetOwner()->GetInstigatorController(), this, BPDamageType);
		}
		if(true)// 这里判断下弹种来决定是否销毁还是过穿
		{
			this->Destroy();
		}
	}
}
