// Fill out your copyright notice in the Description page of Project Settings.


#include "myComponents/MSGearManager.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"// Used by Replicated and GetLifetimeReplicatedProps()

#include"Characters/Ms.h"
#include"Gears/Armor.h"
#include"Gears/MsDevice.h"
#include"Gears/SpecialGear.h"
#include"Gears/MsWeapon.h"

// Sets default values for this component's properties
UMSGearManager::UMSGearManager()
{
	// ...
	MasterWeaponSocketName = "MasterWeaponSocket";
	OffhandWeaponSocketName = "OffhandWeaponSocket";
	LeftHandSocketName = "LeftHandSocket";
	RightHandSocketName = "RightHandSocket";
	LeftLegSocketName = "LeftLegSocket";
	RightLegSocketName = "RightLegSocket";

	//this->SetNetAddressable();//Make DSO components net addressable
	//this->SetIsReplicated(true);// Enable replication by default
}

void UMSGearManager::InitFromBP()
{
	// MasterWeapon
	if (myOwner && myOwner->GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;// 创建生成参数
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;// 设置参数：有碰撞时仍要生成
		MasterWeapon = GetWorld()->SpawnActor<AMsWeapon>(BPMasterWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (MasterWeapon) {
			MasterWeapon->SetOwner(myOwner);
			MasterWeapon->AttachToComponent(myOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MasterWeaponSocketName);
			MasterWeapon->SetHolder(myOwner);
		}
	
		// OffHandWeapon
		OffhandWeapon = GetWorld()->SpawnActor<AMsWeapon>(BPOffhandWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (OffhandWeapon) {
			OffhandWeapon->SetOwner(myOwner);
			OffhandWeapon->AttachToComponent(myOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MasterWeaponSocketName);
			OffhandWeapon->SetHolder(myOwner);
		}
		
		// SpecialGear
		LeftHandGear = GetWorld()->SpawnActor<ASpecialGear>(BPLeftHandGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		RightHandGear = GetWorld()->SpawnActor<ASpecialGear>(BPRightHandGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		LeftLegGear = GetWorld()->SpawnActor<ASpecialGear>(BPLeftLegGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		RightLegGear = GetWorld()->SpawnActor<ASpecialGear>(BPRightLegGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (LeftHandGear) {
			LeftHandGear->SetOwner(myOwner);
			LeftHandGear->AttachToComponent(myOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandSocketName);
			LeftHandGear->SetHolder(myOwner);
		}
		if (RightHandGear) {
			RightHandGear->SetOwner(myOwner);
			RightHandGear->AttachToComponent(myOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandSocketName);
			RightHandGear->SetHolder(myOwner);
		}
		if (LeftLegGear) {
			LeftLegGear->SetOwner(myOwner);
			LeftLegGear->AttachToComponent(myOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftLegSocketName);
			LeftLegGear->SetHolder(myOwner);
		}
		if (RightLegGear) {
			RightLegGear->SetOwner(myOwner);
			RightLegGear->AttachToComponent(myOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightLegSocketName);
			RightLegGear->SetHolder(myOwner);
		}
	}
	
}

// Called when the game starts
void UMSGearManager::BeginPlay()
{
	Super::BeginPlay();
	// ...
	myOwner = Cast<AMS>(GetOwner());// 初始化组件指针
	InitFromBP();

}

void UMSGearManager::UseMasterWeapon()
{
	if(this->MasterWeapon&&this->MasterWeapon->Type < WeaponType::MS_Melee)
	{
		this->MasterWeapon->StartFire();
	}
	else
	{
		
	}
}

void UMSGearManager::StopMasterWeapon()
{
	if(this->MasterWeapon&&this->MasterWeapon->Type < WeaponType::MS_Melee)
	{
		this->MasterWeapon->StopFire();
	}
}

void UMSGearManager::SetMasterWeapon()
{
	
}

void UMSGearManager::SetOffhandWeapon()
{
	
}

void UMSGearManager::SetBodyGears()
{
	
}

void UMSGearManager::DropMasterWeapon()
{
	if (MasterWeapon) {
		this->MasterWeapon->SetOwner(nullptr);
		this->MasterWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		this->MasterWeapon = nullptr;
	}
}

void UMSGearManager::DropOffhandWeapon()
{
	if (OffhandWeapon) {
		this->OffhandWeapon->SetOwner(nullptr);
		this->OffhandWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		this->OffhandWeapon = nullptr;
	}
}

void UMSGearManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMSGearManager, MasterWeapon);
	DOREPLIFETIME(UMSGearManager, OffhandWeapon);
	DOREPLIFETIME(UMSGearManager, LeftHandGear);
	DOREPLIFETIME(UMSGearManager, RightHandGear);
	DOREPLIFETIME(UMSGearManager, LeftLegGear);
	DOREPLIFETIME(UMSGearManager, RightLegGear);
}
