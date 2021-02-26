// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ParriedNotifyState.h"

#include "Characters/MS.h"
#include "myComponents/MSGearManager.h"
#include "Gears/MsWeapon.h"

void UParriedNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C&&C->GearManager&&C->GearManager->MasterWeapon) {
			C->GearManager->MasterWeapon->OnParriedEnableChanged(true);
		}
	}
}

void UParriedNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C && C->GearManager && C->GearManager->MasterWeapon) {
			C->GearManager->MasterWeapon->OnParriedEnableChanged(false);
		}
	}
}
