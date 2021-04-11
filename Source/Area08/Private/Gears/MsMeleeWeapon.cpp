// Fill out your copyright notice in the Description page of Project Settings.


#include "Gears/MsMeleeWeapon.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"// 碰撞盒子 用于攻击判定
#include "PhysicalMaterials/PhysicalMaterial.h"// 用于判断物理材质

#include "Area08/Area08.h"

AMsMeleeWeapon::AMsMeleeWeapon() {

	AttackBox = CreateDefaultSubobject<UBoxComponent>("AttackBoxComp");
	if (this->MeshComponent) {
		AttackBox->SetupAttachment(this->MeshComponent, "Attack");
	}
	AttackBox->SetCollisionProfileName("Weapon");
	AttackBox->SetNotifyRigidBodyCollision(true);// 生成撞击事件，不然无法触发碰撞
	AttackBox->SetBoxExtent(FVector(5));

	StaggeredBox = CreateDefaultSubobject<UBoxComponent>("BlockedBoxComp");
	if (this->MeshComponent) {
		StaggeredBox->SetupAttachment(this->MeshComponent, "Staggered");
	}
	StaggeredBox->SetCollisionProfileName("WeaponOther");
	StaggeredBox->SetNotifyRigidBodyCollision(true);
	StaggeredBox->SetBoxExtent(FVector(5));

	ParriedBox = CreateDefaultSubobject<UBoxComponent>("ParriedBoxComp");
	if (this->MeshComponent) {
		ParriedBox->SetupAttachment(this->MeshComponent, "Parried");
	}
	ParriedBox->SetCollisionProfileName("WeaponOther");
	ParriedBox->SetNotifyRigidBodyCollision(true);
	ParriedBox->SetBoxExtent(FVector(5));
}

void AMsMeleeWeapon::OnHit(UPrimitiveComponent* OverlappedComponent, 
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& Hit)// 武器命中要做的事情
{
	if (HitActor) {
		EPhysicalSurface tempSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Hit generate")), false);
		switch (tempSurfaceType)
		{
		case MS_HEAD:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Head hit.")), false);
			break;
		case MS_BODY:
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Body hit.")), false);
			break;
		default:
			break;
		}

		if (AudioPlayComponent) {
			AudioPlayComponent->SetPitchMultiplier(FMath::RandRange(.5f, 4.f));

			//AudioPlayComponent->Play();
		}
	}
}

void AMsMeleeWeapon::OnParry(UPrimitiveComponent* OverlappedComponent, 
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)// 被弹反要做的事情
{
	OnParriedChanged.Broadcast(this, 0);// 委托
}

void AMsMeleeWeapon::OnStaggered(UPrimitiveComponent* OverlappedComponent, 
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)// 武器弹刀要做的事情
{	
	//bStaggered.Broadcast(this, 0.0f, 0.0f, 0.0f, InstigatedBy, DamageCauser);// 委托
}

void AMsMeleeWeapon::OnAttackEnableChanged(bool Enable)
{
	if (Enable) {
		AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMsMeleeWeapon::OnStaggeredEnableChanged(bool Enable)
{
	if (Enable) {
		StaggeredBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		StaggeredBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMsMeleeWeapon::OnParriedEnableChanged(bool Enable)
{
	if (Enable) {
		ParriedBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		ParriedBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMsMeleeWeapon::MeleeBreak()
{
	this->OnAttackEnableChanged(false);
	this->OnParriedEnableChanged(false);
	this->OnStaggeredEnableChanged(false);
}

void AMsMeleeWeapon::Melee()
{
	// 播放人物专有的武器动画
}

void AMsMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMsMeleeWeapon::OnHit);
	StaggeredBox->OnComponentBeginOverlap.AddDynamic(this, &AMsMeleeWeapon::OnParry);
	ParriedBox->OnComponentBeginOverlap.AddDynamic(this, &AMsMeleeWeapon::OnParry);

	// 如果碰撞开关放在构造函数里，有时会失效，可能是蓝图继承的关系
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaggeredBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParriedBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
