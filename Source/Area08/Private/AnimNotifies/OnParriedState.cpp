// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/OnParriedState.h"
#include "Characters/MS.h"
#include "myComponents/MSGearManager.h"
#include "Gears/MsWeapon.h"

void UOnParriedState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C) {// 瘫痪角色的移动和操控输入
			C->DisablePlayerInput(true);
			C->DiableMsMovement(true);
			//C->bParried = true;
		}
		if (C && C->GearManager && C->GearManager->MasterWeapon) {
			//C->GearManager->MasterWeapon->MeleeBreak();// 武器失效
		}
	}
}

void UOnParriedState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp) {
		AMS* C = Cast<AMS>(MeshComp->GetOwner());
		if (C) {
			C->DisablePlayerInput(false);
			C->DiableMsMovement(false);
			C->bParried = false;// 结束人物被弹反的动画
		}
	}
}
