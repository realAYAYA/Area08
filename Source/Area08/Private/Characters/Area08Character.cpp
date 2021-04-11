// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Area08Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AArea08Character::AArea08Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;
	BaseRollRate = 45.0f;

	//InitCamera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FirstPersonCameraComponent->SetupAttachment(SpringArm);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	/*GravityManager = CreateDefaultSubobject<UPhysicsThrusterComponent>("GravityManager");
	GravityManager->SetupAttachment(GetCapsuleComponent());
	GravityManager->SetAutoActivate(true);
	GravityManager->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(FVector(1, 0, 0)));
	GravityManager->ThrustStrength = GetCapsuleComponent()->GetMass() * 980.0f;*/
}

// Called when the game starts or when spawned
void AArea08Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AArea08Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);

}
