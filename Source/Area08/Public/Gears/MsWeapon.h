// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Gears/Gear.h"
#include "MsWeapon.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FWeaponMontage : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float Duration;

	UPROPERTY(EditAnywhere)
	FString Desc;
};

UENUM()
enum class WeaponType : uint8
{
	// Gun
	MS_Riflegun,
	MS_Mechinegun,

	MS_Handgun,
	MS_Shotgun,

	MS_AntiSheep,

	// Melee
	MS_Melee,
	MS_Great,
	MS_SuperGreat,
	MS_Small,
};

UCLASS()
class AREA08_API AMsWeapon : public AGear
{
	GENERATED_BODY()

public:
	AMsWeapon();

	/** 枪械部分*/
	virtual void StartFire();
	virtual void StopFire();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnFire();
	
	void PlayFireEffects()const;// 播放开火特效
	
	/** 近战攻击部分，可被弹刀，被招架 判定开启*/
	virtual void Melee() {}
	virtual void MeleeBreak() {}
	void PlayMeleeEffects();// 播放近战特效

	virtual void OnAttackEnableChanged(bool Enable) {}
	virtual void OnStaggeredEnableChanged(bool Enable) {}
	virtual void OnParriedEnableChanged(bool Enable) {}

	/** 武器技能*/
	virtual void Skill1(){}
	virtual void Skill2(){}
	virtual void Skill3(){}
	virtual void Skill4(){}
	
	/** 联网*/
	UFUNCTION(Server, Reliable, WithValidation)
	void SeverFire();
	UPROPERTY(ReplicatedUsing=OnRep_Fire)
	int FireRep;// 监测变量执行回调，服务器端开火同步到客户端
	UFUNCTION()
	void OnRep_Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	/** 基本组件*/
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	WeaponType Type;

	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	class USceneComponent* Root;// 用作根，方便编辑

	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MsWeapon", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponMontageDataTable;

	class UAudioComponent* AudioPlayComponent;// 声音播放组件

	/** 枪械部分*/
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MsWeapon")
	class UParticleSystem* TracerEffect;// 枪火的延展效果

	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	class USoundBase* NoAmmoSound;// 弹夹空音效

	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	class USoundBase* FireSound;// 开火音效

	/* Debug, BPDebug, These variables will be deleted soon*/
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	TSubclassOf<class ABullet> Bullet;

	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	float FiringRate;
	
	FTimerHandle TimeHandle_TimeBetweenShoots;
	float TimeBetweenShoots;
	float LastFireTime;
		
	// UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	// TSubclassOf<class UCameraShake> FireCameraShake;// 开火导致镜头摇动

	/** 近战攻击部分，可被弹刀，被招架 判定开启*/	
	/* 人物指定武器动画，在特定条件下使用人物播放*/
	// 攻击动画 // 被弹反动画// 被弹刀动画
	/* 武器特效*/
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MsWeapon", meta = (AllowPrivateAccess = "true"))
	//UParticleSystem* MuzzleEffect;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MsWeapon", meta = (AllowPrivateAccess = "true"))
	//UParticleSystem* HitEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MsWeapon", meta = (AllowPrivateAccess = "true"))
	USoundBase* HitSound;

	/* 碰撞盒子在武器上的位置，朝向属性*/
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	FName MeleeBoxSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	FVector AttackBoxScale;
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	FVector StaggerBoxScale;
	UPROPERTY(EditDefaultsOnly, Category = "MsWeapon")
	FVector ParryBoxScale;

	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return MeshComponent; }

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
