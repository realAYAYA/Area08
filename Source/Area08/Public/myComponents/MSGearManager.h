// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MSGearManager.generated.h"

UCLASS( ClassGroup=(MS), meta=(BlueprintSpawnableComponent) )
class AREA08_API UMSGearManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMSGearManager();

	/* TestFromBP*/
	void InitFromBP();

	/* Debug, BPDebug, These variables will be deleted soon*/
	UPROPERTY(EditDefaultsOnly, Category = "GearSocket")
	TSubclassOf<class AMsWeapon>BPMasterWeapon;

//
	UPROPERTY(EditDefaultsOnly, Category = "GearSocket")
	TSubclassOf<AMsWeapon> BPOffhandWeapon;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
//		class AArmor* Armor;

	UPROPERTY(EditDefaultsOnly, Category = "GearSocket")
	TSubclassOf<class ASpecialGear> BPLeftHandGear;

	UPROPERTY(EditDefaultsOnly, Category = "GearSocket")
	TSubclassOf<ASpecialGear> BPRightHandGear;

	UPROPERTY(EditDefaultsOnly, Category = "GearSocket")
	TSubclassOf<ASpecialGear> BPLeftLegGear;

	UPROPERTY(EditDefaultsOnly, Category = "GearSocket")
	TSubclassOf<ASpecialGear> BPRightLegGear;

	UPROPERTY(EditDefaultsOnly, Category = "GearSocket")
	TSubclassOf<ASpecialGear> BPBackPack;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
//	TArray<class AMsDevice*> BPMsDevices;
//	//怎么去管理很多个机载设备呢？---限定玩家可以手动激活的设备数目为 除主副手装备外 4个，在四肢除分别安装
//	//即便能安装很多设备，玩家又如何仅靠键鼠去操控呢？更多的机载设备提供 被动型 的功能
//
	FName MasterWeaponSocketName;
	FName OffhandWeaponSocketName;
	FName LeftHandSocketName;
	FName RightHandSocketName;
	FName LeftLegSocketName;
	FName RightLegSocketName;

	UPROPERTY(Replicated)
	AMsWeapon* MasterWeapon;
//
	UPROPERTY(Replicated)
	AMsWeapon* OffhandWeapon;
//
//	UPROPERTY(Replicated)
//		class AArmor* Armor;
//
	UPROPERTY(Replicated)
	ASpecialGear* LeftHandGear;

	UPROPERTY(Replicated)
	ASpecialGear* RightHandGear;

	UPROPERTY(Replicated)
	ASpecialGear* LeftLegGear;

	UPROPERTY(Replicated)
	ASpecialGear* RightLegGear;
//
//	UPROPERTY(Replicated)
//	TArray<class AMsDevice*> MsDevices;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void UseMasterWeapon();
	
	void SetMasterWeapon();
	void SetOffhandWeapon();
	void SetBodyGears();

	void DropMasterWeapon();
	void DropOffhandWeapon();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
