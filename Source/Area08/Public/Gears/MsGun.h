// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gears/MsWeapon.h"
#include "MsGun.generated.h"

/**
 * 
 */
UCLASS()
class AREA08_API AMsGun : public AMsWeapon
{
	GENERATED_BODY()

public:
	AMsGun();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	FName MuzzleSocketName;

	/* Debug, BPDebug, These variables will be deleted soon*/
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TSubclassOf<class ABullet> Bullet;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float FiringRate;
	float TimeBetweenShoots;
	FTimerHandle TimeHandle_TimeBetweenShoots;
	float LastFireTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnFire();

	virtual void StartFire()override;
	virtual void StopFire()override;

};
