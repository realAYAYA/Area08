// Fill out your copyright notice in the Description page of Project Settings.


#include "myComponents/RayTestComponent.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Gears/MsWeapon.h"



// Sets default values for this component's properties
URayTestComponent::URayTestComponent()
{
	// ...
	Range = 10000.0f;
	Activated = true;
	DrawLine = true;
	Color = FColor::Red;
}

void URayTestComponent::Tracing()
{
	if (!Activated)
		return;

	FVector BeginLocation;
	FRotator BeginRotation;
	AActor* MyOwner = GetOwner();
	if(MyOwner){
		MyOwner->GetActorEyesViewPoint(BeginLocation, BeginRotation);
		if (CameraComponent) {
			BeginLocation = CameraComponent->GetComponentLocation();
		}
		FVector TraceEnd = BeginLocation + (BeginRotation.Vector() * Range);
		FHitResult Hit;
		FCollisionQueryParams QueryParam;
		QueryParam.AddIgnoredActor(MyOwner);// Ignore Character
		QueryParam.bTraceComplex = true;
		if (GetWorld()->LineTraceSingleByChannel(Hit, BeginLocation, TraceEnd, ECC_Visibility, QueryParam)) {
			AActor* HitActor = Hit.GetActor();
			AMsWeapon* Weapon = Cast<AMsWeapon>(HitActor);
			if (Weapon) {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, Weapon->Name.ToString(), false);
			}
		}
		if (DrawLine) {
			DrawDebugLine(GetWorld(), BeginLocation, TraceEnd, Color, false, 1.0f, 0, 1.0f);
		}
	}
}

// Called when the game starts
void URayTestComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}

void URayTestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
