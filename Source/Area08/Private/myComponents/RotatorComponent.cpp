// Fill out your copyright notice in the Description page of Project Settings.


#include "myComponents/RotatorComponent.h"

// Sets default values for this component's properties
URotatorComponent::URotatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}

void URotatorComponent::Turn(float Val)
{
	AActor* Owner = GetOwner();
	if (Owner) {
		FVector NewVector = RotateAngleAxis(Owner->GetActorForwardVector(), Val, Owner->GetActorUpVector());
		FRotator NewRotator = NewVector.ToOrientationRotator();
		FQuat QuatRotation = FQuat(NewRotator);
		//Owner->SetActorRotation(QuatRotation);
		Owner->SetActorRotation(NewRotator);
	}
}

void URotatorComponent::LookUp(float Val)
{
	AActor* Owner = GetOwner();
	if (Owner) {
		FVector NewVector = RotateAngleAxis(Owner->GetActorForwardVector(), Val, Owner->GetActorRightVector());
		FRotator NewRotator = NewVector.ToOrientationRotator();
		//FQuat QuatRotation = FQuat(NewRotator);
		Owner->SetActorRotation(NewRotator);
	}
}

void URotatorComponent::Roll(float Val)
{
	AActor* Owner = GetOwner();
	if (Owner) {
		FVector NewVector = RotateAngleAxis(Owner->GetActorRightVector(), Val, Owner->GetActorForwardVector());
		FRotator NewRotator = NewVector.ToOrientationRotator();
		//FQuat QuatRotation = FQuat(NewRotator);
		Owner->SetActorRotation(NewRotator);
	}
}

FVector URotatorComponent::RotateAngleAxis(const FVector& V, const float Angle, const FVector& Axis)
{
	float S, C;
	FMath::SinCos(&S, &C, FMath::DegreesToRadians(Angle));

	// Rodrigues' rotation formula: Vrot = cosA*V + (1 - cosA)(V*K)K +sinA*K x V
	const float OMC = 1 - C;
	FVector Vrot = C * V + OMC * Dot(V, Axis) * V + S * Cross(Axis, V);
	return Vrot;
}

float URotatorComponent::Dot(const FVector& V, const FVector& K)
{
	return V.X * K.X + V.Y * K.Y + V.Z * K.Z;
}

FVector URotatorComponent::Cross(const FVector& V, const FVector& K)
{
	return FVector(V.Y * K.Z - V.Z * K.Y, V.Z * K.X - V.X * K.Z, V.X * K.Y - V.Y * K.X);
}
