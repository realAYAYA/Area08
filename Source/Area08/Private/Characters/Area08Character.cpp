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
	SpringArm->TargetArmLength = 600.0f;
    SpringArm->bDoCollisionTest = true;
    SpringArm->ProbeSize = 15.0f;
    SpringArm->ProbeChannel = ECollisionChannel::ECC_Camera;
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bInheritPitch = true;
    SpringArm->bInheritYaw = true;
    SpringArm->bInheritRoll = true;
	SpringArm->SetupAttachment(RootComponent);

	this->bUseControllerRotationYaw=false;
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FirstPersonCameraComponent->SetupAttachment(SpringArm);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	//FirstPersonCameraComponent->bUsePawnControlRotation = true;

	/*GravityManager = CreateDefaultSubobject<UPhysicsThrusterComponent>("GravityManager");
	GravityManager->SetupAttachment(GetCapsuleComponent());
	GravityManager->SetAutoActivate(true);
	GravityManager->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(FVector(1, 0, 0)));
	GravityManager->ThrustStrength = GetCapsuleComponent()->GetMass() * 980.0f;*/

	// Init View Limit
	CameraPitchMin = -89.0f;
	CameraPitchMax = 89.0f;
	MinVelocityToRotateMesh = 2.0f;
	RotationInterpSpeed = 5.0f;
}

// Called when the game starts or when spawned
void AArea08Character::BeginPlay()
{
	Super::BeginPlay();
	
	MeshStartRotation = GetMesh()->GetRelativeRotation();// Init
}

void AArea08Character::AddCameraPitchInput(float UpdateRate, float ScaleValue)
{
	if (SpringArm != NULL)
	{
		FRotator CameraRelativeRot = SpringArm->GetRelativeRotation();
		float CameraNewPitch = FMath::ClampAngle(CameraRelativeRot.Pitch + ScaleValue * UpdateRate, CameraPitchMin, CameraPitchMax);
		CameraRelativeRot.Pitch = CameraNewPitch;
		SpringArm->SetRelativeRotation(CameraRelativeRot);
	}
}

void AArea08Character::AddCameraYawInput(float UpdateRate, float ScaleValue)
{
	if (SpringArm != NULL)
	{
		SpringArm->AddRelativeRotation(FRotator(0.0f, ScaleValue * UpdateRate, 0.0f));
	}
}

// Called to bind functionality to input
void AArea08Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);

}
