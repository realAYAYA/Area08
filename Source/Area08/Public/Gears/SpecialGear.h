// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gears/Gear.h"
#include "SpecialGear.generated.h"

/**
 * 
 */
UCLASS()
class AREA08_API ASpecialGear : public AGear
{
	GENERATED_BODY()
public:
	ASpecialGear();
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComponent;

	virtual void Activate() {};
	virtual void Passive() {};

private:

};
