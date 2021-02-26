// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AttackingNotifyState.h"

#include "Characters/MS.h"
#include "myComponents/MSGearManager.h"
#include "Gears/MsWeapon.h"

void UAttackingNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C && C->GearManager && C->GearManager->MasterWeapon) {
			C->GearManager->MasterWeapon->OnAttackEnableChanged(true);
		}
	}
}

void UAttackingNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C && C->GearManager && C->GearManager->MasterWeapon) {
			C->GearManager->MasterWeapon->OnAttackEnableChanged(false);
		}
	}
}
