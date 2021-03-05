// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Area08Character.h"
#include "MS.generated.h"

/**
 * 
 */
UCLASS()
class AREA08_API AMS : public AArea08Character
{
	GENERATED_BODY()
	
public:
	AMS();

	class UMyUserWidget* HUD;// Test HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UMyUserWidget> WidgetClass;
	void InitHUD();
	UFUNCTION()
	FLinearColor UpdateHUD();// 用于将HUD的变量进行'代理'

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	class URayTestComponent* LineTracer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	class UMSGearManager* GearManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	class UMsHealthComponent* HealthManager;
	UFUNCTION()// Events
	void OnHealthChanged(UMsHealthComponent* OwnerHealthComp, float Health, float HeathDelta, 
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MsAnimotion, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MsAnimotion, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ParriedMontage;

	/* Charactors' status change*/
	void Stuned(bool Val);

	void DisablePlayerInput(bool Val);

	void DiableMsMovement(bool Val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TestTouch();

	void StartFire();
	void StopFire();
	void Melee();// 近战
	void PunchWithWeapon();
	void RFire();

public:
	/* 机甲状态类变量*/
	UPROPERTY(Replicated)
	bool bDied;
	void SetDeath();
	
	UPROPERTY(Replicated)
	bool bParried;// 角色被弹反
	UFUNCTION()// Events
	void PlayParriedMontage(class AMsMeleeWeapon* Weapon, float val);

	UPROPERTY(Replicated)
	bool bStaggered;// 角色被弹刀
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE bool GetStandingMontagePlaying() const { return bStandingMontagePlaying; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetDiedMontagePlay() const { return bDied; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetParriedMontagePlay() const { return bParried; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetStaggeredMontagePlay() const { return bStaggered; }
};
