// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ParriedNotifyState.generated.h"

/**
 * 将角色设置为可以被弹反的状态，并在动画结束的时候关闭
 * 如果次动画被提前打断，请记得及时在其他地方将变量修改
 */
UCLASS()
class AREA08_API UParriedNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
