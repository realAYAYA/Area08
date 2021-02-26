// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Rotator.h"
#include "RotatorComponent.generated.h"


UCLASS( ClassGroup=(MS), meta=(BlueprintSpawnableComponent) )
class AREA08_API URotatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotatorComponent();

	void Turn(float Val);
	void LookUp(float Val);
	void Roll(float Val);

	/* Rodrigues' rotation formula: Vrot = cosA*V + (1 - cosA)(V*K)K +sinA*K x V*/
	FVector RotateAngleAxis(const FVector& V, const float Angle, const FVector& Axis);

	float Dot(const FVector& V, const FVector& K);
	FVector Cross(const FVector& V, const FVector& K);
};