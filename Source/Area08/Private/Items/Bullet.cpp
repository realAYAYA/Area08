// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "Characters/MS.h"

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
}

void ABullet::BeginPlay() {
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnHit);//设置事件触发委托

	SetLifeSpan(8.0f);//设置自我销毁时间,在BeginPlay里可以，在构造函数里调用会报错
}

void ABullet::OnHit(UPrimitiveComponent* OverlappedComponent,
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitActor) {
		/*AMS* C = Cast<AMS>(HitActor);
		if (C) {
			UGameplayStatics::ApplyPointDamage(C, Damage,
				this->GetActorForwardVector(), SweepResult, this->GetOwner()->GetInstigatorController(), this, DamageType);
		}*/

		this->Destroy();
	}
}
