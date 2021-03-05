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

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	WeaponType Type;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USceneComponent* Root;// 用作根，防止武器模型为根无法设置姿态

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponMontageDataTable;

	class UAudioComponent* AudioPlayComponent;// 声音播放组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = "true"))
	USoundBase* UnKnowSound;

public:
	// 枪械武器的方法接口
	virtual void StartFire() {}
	virtual void StopFire() {}

	// 近战武器的方法接口
	virtual void Melee() {}
	virtual void MeleeBreak() {}

	virtual void OnAttackEnableChanged(bool Enable) {}
	virtual void OnStaggeredEnableChanged(bool Enable) {}
	virtual void OnParriedEnableChanged(bool Enable) {}
	
};
