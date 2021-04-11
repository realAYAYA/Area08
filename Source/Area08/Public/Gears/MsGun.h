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
	
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets")
	class UParticleSystem* TracerEffect;// 枪火的延展效果

	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	class USoundBase* NoAmmoSound;// 弹夹空音效

	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	class USoundBase* FireSound;// 开火音效

	
	/* Debug, BPDebug, These variables will be deleted soon*/
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	TSubclassOf<class ABullet> Bullet;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float FiringRate;
	
	FTimerHandle TimeHandle_TimeBetweenShoots;
	float TimeBetweenShoots;
	float LastFireTime;
	
	
	// UPROPERTY(EditDefaultsOnly, Category = "Components")
	// TSubclassOf<class UCameraShake> FireCameraShake;// 开火导致镜头摇动

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnFire();

	virtual void StartFire()override;
	virtual void StopFire()override;

	void PlayEffects();		

	/* 联网**/
	UFUNCTION(Server, Reliable, WithValidation)
    void SeverFire();

	UPROPERTY(ReplicatedUsing=OnRep_Fire)
	int FireRep;// 监测变量执行回调，服务器端开火同步到客户端
	UFUNCTION()
	void OnRep_Fire();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
