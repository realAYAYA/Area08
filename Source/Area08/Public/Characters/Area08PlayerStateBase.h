// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Area08PlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class AREA08_API AArea08PlayerStateBase : public APlayerState
{
	GENERATED_BODY()
public:
	AArea08PlayerStateBase();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent;}

	UAbilitySystemComponent* AbilitySystemComponent;
};
