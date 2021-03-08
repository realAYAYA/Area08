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

	/* 存放了机甲的一些默认的基本蒙太奇动画表格*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MsAnimotion, meta = (AllowPrivateAccess = "true"))
	class UDataTable* MSMontageTable;

	/* Charactors' status change*//* 机甲状态类变量及相关的函数*/
	void Stuned(bool Val);

	void DisablePlayerInput(bool Val);

	void DiableMsMovement(bool Val);

	UPROPERTY(Replicated)
	bool bMoveable;// 角色无法移动
	
	UPROPERTY(Replicated)
	bool bParried;// 角色被弹反,可以被处决

	UPROPERTY(Replicated)
	bool bStaggered;// 角色被弹反,可以被处决

	UPROPERTY(Replicated)
	bool bStuned;// 角色瘫痪，不能移动，不能进行任何操作

	UPROPERTY(Replicated)
	bool Attacking;// 角色正在攻击，不能移动，不能进行其他操作
	
	UPROPERTY(Replicated)
	bool bDied;

	bool Moveable();// 用此函数来归纳角色状态，决定角色能不能移动
	bool Runable();
	bool Turnable();

	void SetDeath();
	
	UFUNCTION()// Events
    void PlayParriedMontage(class AMsMeleeWeapon* Weapon, float val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TestTouch();

	void StartFire();
	void StopFire();
	void Melee();// 近战
	void PunchWithWeapon();
	void RFire();

	/** MS move input*/
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn, look up/down, roll at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Roll(float Rate);	

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
    FORCEINLINE bool GetStunedMontagePlay() const { return bStuned; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetStaggeredMontagePlay() const { return bStaggered; }

	UFUNCTION(BlueprintCallable)
    FORCEINLINE bool GetAttackMontagePlay() const { return Attacking; }
};
