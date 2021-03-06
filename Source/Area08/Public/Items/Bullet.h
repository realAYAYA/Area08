// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Bullet.generated.h"

UCLASS()
class AREA08_API ABullet : public AItem
{
	GENERATED_BODY()

public:
	ABullet();
	

	UPROPERTY(EditAnywhere, Category = "Properties")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<class UArea08DamageType> BPDamageType;
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	class UArea08DamageType* DamageType;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float LifeTime;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
