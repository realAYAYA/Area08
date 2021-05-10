// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Area08PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class AREA08_API AArea08PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	/**
	* 对于玩家控制的Character且ASC位于Pawn, 我一般在服务端Pawn的PossessedBy()函数中初始化,
	* 在客户端PlayerController的AcknowledgePossession()函数中初始化.
	* @param P  ...
	*/
	virtual void AcknowledgePossession(APawn* P) override;
};
