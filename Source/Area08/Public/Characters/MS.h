// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Area08Character.h"
#include "MS.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class MsStatus : uint8
{
	Normal,
	Moving,// 角色无法移动
	bParried,// 角色被弹反,可以被处决
	bStaggered,// 角色被弹反,可以被处决
	bStuned,// 角色瘫痪，不能移动，不能进行任何操作
	Attacking,// 角色正在攻击，不能移动，不能进行其他操作
	Dodging,// 角色正在闪避
	bDied,
};

UCLASS()
class AREA08_API AMS : public AArea08Character
{
	GENERATED_BODY()

public:
	AMS();

	/** 组件指针*/
	class UMyUserWidget* HUD;// Test HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UMyUserWidget> WidgetClass;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	class URayTestComponent* LineTracer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	class UMSGearManager* GearManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	class UMsHealthComponent* HealthManager;
	UFUNCTION()// HealthManager Events
	void OnHealthChanged(UMsHealthComponent* OwnerHealthComp, float Health, float HeathDelta, 
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MsAnimotion, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;

	/** 存放了机甲的一些默认的基本蒙太奇动画表格*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MsAnimotion, meta = (AllowPrivateAccess = "true"))
	class UDataTable* MSMontageTable;
	
	/** 时间轴管理*/
	float DefaultDodgeTime;// 闪避过程需要的时间
	float DodgeTime;// 闪避过程需要的时间
	
	FTimerHandle SprintTimerHandle;// 冲刺时间轴
    
	FTimerHandle TimeHandle_TimeBetweenPress;// 冲刺充能	

	/** Charactors' status change 机甲状态类变量及相关的函数*/	
	UPROPERTY(Replicated)
	MsStatus myStatus;

	bool Moveable();// 用此函数来归纳角色状态，决定角色能不能移动
	bool Runable();
	bool Turnable();

	UFUNCTION()
	void SetDeath();

	UFUNCTION()// Events
    void PlayParriedMontage(class AMsMeleeWeapon* Weapon, float val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TestTouch();

	/** MS battle input*/
	void StartFire();
	void StopFire();
	void Melee();// 近战
	void RFire();	

	void Dodge();// 角色根据当前速度方向进行前冲，滑步，闪避（怎么叫都可以）
	void Tick_Dodge(float DeltaTime);// 计时器回调方法，用来计时
	
	/** MS move input*/
	/** Handles moving forward/backward || stafing movement, left and right */
	void MoveForward(float Val);	
	void MoveRight(float Val);

	/** Handles moving fast || sliding */
	void SprintBegin();
	void SprintEnd();
		
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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE MsStatus GetStatusMontagePlay() const { return myStatus; }

};
