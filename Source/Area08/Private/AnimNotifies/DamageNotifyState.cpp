// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/DamageNotifyState.h"

#include "Characters/MS.h"
#include "myComponents/MSGearManager.h"
#include "Gears/MsWeapon.h"

void UDamageNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C && C->GearManager && C->GearManager->MasterWeapon) {
			C->GearManager->MasterWeapon->OnAttackEnableChanged(true);
		}
	}
}

void UDamageNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C && C->GearManager && C->GearManager->MasterWeapon) {
			C->GearManager->MasterWeapon->OnAttackEnableChanged(false);
		}
	}
}