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

UENUM(BlueprintType)
enum class DriveMode : uint8
{
	Walk,
	Fly
};

UCLASS()
class AREA08_API AMS : public AArea08Character
{
	GENERATED_BODY()

public:
	AMS();

	virtual void UpdateMeshRotation(float DeltaTime)override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 蓝图内联函数*/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UMsAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent;}
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UMSGearManager* GetGearsManager() const { return GearManager; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UMsHealthComponent* GetHealthComponent() const { return HealthManager; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE MsStatus GetStatusMontagePlay() const { return myStatus; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE DriveMode GetDriveModeMontagePlay() const { return myDriveMode; }

	/**
	* Called every Tick.
	* @param DeltaTime
	*/
	virtual void Tick(float DeltaTime) override;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TestTouch();

	/** 机体状态方法*/
	bool Moveable()const;// 用此函数来归纳角色状态，决定角色能不能移动
	bool Runable()const;
	bool Turnable()const;

	/** MS battle input*/
	void StartFire();
	void StopFire();
	void RFire();
	
	/** 近战攻击部分，可被弹刀，被招架 判定开启*/
	void Melee();// 近战

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnParry(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnStaggered(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void UpdateCollisionBox();// 装备武器的时候，将近战碰撞盒初始化到武器上，没有武器的时候，将近战碰撞盒重置到机甲拳头
	void OnAttackEnableChanged(bool Enable);
	void OnStaggeredEnableChanged(bool Enable);
	void OnParriedEnableChanged(bool Enable);
	UFUNCTION()// Events
	void PlayParriedMontage(class AMsWeapon* Weapon, float val);
	
		
	/** MS move input*/
	void MoveForward(float Val);	
	void MoveRight(float Val);
	
	void SprintBegin();
	void SprintEnd();

	void Dodge();// 角色根据当前速度方向进行前冲，滑步，闪避（怎么叫都可以）
		
	/**
	* Called via input to turn, look up/down, roll at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Roll(float Rate);

	UFUNCTION()
	void SetDeath();

	/**
	* 基于UE4自带的ASC系统实现的技能管理器和Buff管理器：https://github.com/BillEliot/GASDocumentation_Chinese#intro
	* 对于玩家控制的Character且ASC位于Pawn, 我一般在服务端Pawn的PossessedBy()函数中初始化,
	* 在客户端PlayerController的AcknowledgePossession()函数中初始化.
	* ------------------------------------------------------------------------------------
	* 对于玩家控制的Character且ASC位于PlayerState, 我一般在服务端Pawn的PossessedBy()函数中初始化,
	* 在客户端PlayerController的OnRep_PlayerState()函数中初始化, 这确保了PlayerState存在于客户端上.
	* @param NewController ...
	*/
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:
	/** 基本组件*/
	class UMyUserWidget* HUD;// Test HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UMyUserWidget> WidgetClass;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	class URayTestComponent* LineTracer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	UMSGearManager* GearManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	UMsHealthComponent* HealthManager;
	UFUNCTION()// HealthManager Events
	void OnHealthChanged(UMsHealthComponent* OwnerHealthComp, float Health, float HeathDelta, 
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MSComponent, meta = (AllowPrivateAccess = "true"))
	UMsAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MsAnimotion, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MsAnimotion, meta = (AllowPrivateAccess = "true"))
	class UDataTable* MSMontageTable;// 存放了机甲的一些默认的基本蒙太奇动画表格

	class UAudioComponent* AudioPlayComponent;// 声音播放组件

	/** 近战相关，可被弹刀，被招架 判定开启*/
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	FName MeleeBoxSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	FVector MeleeBoxScale;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* AttackBox;// 攻击判断盒子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* StaggeredBox;// 被弹刀判定盒子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* ParriedBox;// 被招架判, 弹刀定盒子
	
	/** 时间轴管理*/
	FTimerHandle SprintTimerHandle;// 冲刺时间轴
	FTimerHandle TimeHandle_TimeBetweenPress;// 冲刺充能

	/** Charactors' status change 机甲状态类变量及相关的函数*/	
	UPROPERTY(Replicated)
	MsStatus myStatus;

	UPROPERTY(Replicated)
	DriveMode myDriveMode;
};
