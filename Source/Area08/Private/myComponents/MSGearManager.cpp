// Fill out your copyright notice in the Description page of Project Settings.


#include "myComponents/MSGearManager.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"// Used by Replicated and GetLifetimeReplicatedProps()

#include"Gears/Armor.h"
#include"Gears/MsDevice.h"
#include"Gears/SpecialGear.h"
#include"Gears/MsWeapon.h"
#include"Gears/MsMeleeWeapon.h"

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
}

// Called when the game starts
void UMSGearManager::BeginPlay()
{
	Super::BeginPlay();
	// ...
	SetMasterWeapon();
	SetBodyGears();
}

void UMSGearManager::SetMasterWeapon()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner && Owner->GetLocalRole() == ROLE_Authority) {
		FActorSpawnParameters SpawnParams;// 创建生成参数
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;// 设置参数：有碰撞时仍要生成
		MasterWeapon = GetWorld()->SpawnActor<AMsWeapon>(BPMasterWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (MasterWeapon) {
			MasterWeapon->SetOwner(Owner);
			MasterWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MasterWeaponSocketName);

			if (MasterWeapon->Type >= Weapon::MS_Melee) {

			}
		}
	}
}

void UMSGearManager::SetOffhandWeapon()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner && Owner->GetLocalRole() == ROLE_Authority) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		OffhandWeapon = GetWorld()->SpawnActor<AMsWeapon>(BPOffhandWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (OffhandWeapon) {
			OffhandWeapon->SetOwner(Owner);
			OffhandWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MasterWeaponSocketName);
		}
	}
}

void UMSGearManager::SetBodyGears()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner && Owner->GetLocalRole() == ROLE_Authority) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		LeftHandGear = GetWorld()->SpawnActor<ASpecialGear>(BPLeftHandGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		RightHandGear = GetWorld()->SpawnActor<ASpecialGear>(BPRightHandGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		LeftLegGear = GetWorld()->SpawnActor<ASpecialGear>(BPLeftLegGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		RightLegGear = GetWorld()->SpawnActor<ASpecialGear>(BPRightLegGear, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (LeftHandGear) {
			LeftHandGear->SetOwner(Owner);
			LeftHandGear->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandSocketName);
		}
		if (RightHandGear) {
			RightHandGear->SetOwner(Owner);
			RightHandGear->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandSocketName);
		}
		if (LeftLegGear) {
			LeftLegGear->SetOwner(Owner);
			LeftLegGear->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftLegSocketName);
		}
		if (RightLegGear) {
			RightLegGear->SetOwner(Owner);
			RightLegGear->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightLegSocketName);
		}
	}
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
