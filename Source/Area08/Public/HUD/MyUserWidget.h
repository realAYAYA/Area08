// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
class UProgressBar;
class UTextBlock;
class UImage;
UCLASS()
class AREA08_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UImage* HudHead;
	UPROPERTY()
	UImage* HudBody;
	UPROPERTY()
	UImage* HudLA;
	UPROPERTY()
	UImage* HudRA;
	UPROPERTY()
	UImage* HudLL;
	UPROPERTY()
	UImage* HudRL;
};
