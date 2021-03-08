// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/MS.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"// 用于通过资源引用来调用资源

#include "myComponents/MSGearManager.h"
#include "myComponents/MsHealthComponent.h"
#include "myComponents/RayTestComponent.h"
#include "Gears/MsWeapon.h"
#include "Gears/MsGun.h"
#include "Gears/MsMeleeWeapon.h"
#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"// Test HUD


AMS::AMS() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// init MS status
	bMoveable=true;
	bStuned=false;
	Attacking=false;
	bParried = false;
	bStaggered=false;
	bDied = false;
	

	GearManager = CreateDefaultSubobject<UMSGearManager>(TEXT("GearManagement"));
	HealthManager = CreateDefaultSubobject<UMsHealthComponent>(TEXT("HealthComponent"));
	LineTracer= CreateDefaultSubobject<URayTestComponent>(TEXT("LineTracer"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PunchMontageObject(TEXT("AnimMontage'/Game/Combat/Melee/Attack1.Attack1'"));
	if (PunchMontageObject.Succeeded()) {
		AttackMontage = PunchMontageObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> MontageTableObject(TEXT("DataTable'/Game/Test/DataTables/CharacterBaseMontages.CharacterBaseMontages'"));
	if (MontageTableObject.Succeeded()) {
		MSMontageTable = MontageTableObject.Object;
	}

}

bool AMS::Moveable()
{
	if(bMoveable&&!Attacking&&!bStuned&&!bStaggered&&!bParried)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AMS::Runable()
{
	if(true)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool AMS::Turnable()
{
	if(!bStuned&&!bStaggered&&!bParried)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void AMS::OnHealthChanged(UMsHealthComponent* OwnerHealthComp, float Health, float HeathDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bDied)
		return;

	// 更新HUD
	if(this->HUD&&this->HealthManager){this->HUD->UpdateHealth(this->HealthManager);}
	
	if (Health <= 0.0f && !bDied)
	{
		SetDeath();
	}
}

void AMS::BeginPlay()
{
	Super::BeginPlay();

	HealthManager->OnHealthChanged.AddDynamic(this, &AMS::OnHealthChanged);

	if (GearManager && GearManager->MasterWeapon && GearManager->MasterWeapon->Type >= WeaponType::MS_Melee) {
		AMsMeleeWeapon* M = Cast<AMsMeleeWeapon>(GearManager->MasterWeapon);
		if (M) {
			M->OnParriedChanged.AddDynamic(this, &AMS::PlayParriedMontage);
		}
	}
	
	LineTracer->SetCamera(FirstPersonCameraComponent);

	// 加载一个测试用的HUD，后期删掉
	if (WidgetClass) {
		HUD = CreateWidget<UMyUserWidget>(GetWorld(), WidgetClass);
		if (HUD) {
			HUD->Init();
			HUD->AddToViewport();
		}
	}
}

void AMS::Stuned(bool Val)
{

}

void AMS::DisablePlayerInput(bool Val)
{
	//this->DisableInput();
}

void AMS::DiableMsMovement(bool Val)
{

}

void AMS::TestTouch()
{
	// calculate delta for this frame from the rate information
	
	LineTracer->Tracing();
}

void AMS::StartFire()
{
	if (GearManager && GearManager->MasterWeapon) {
		if (GearManager->MasterWeapon->Type < WeaponType::MS_Melee) {
			GearManager->MasterWeapon->StartFire();
		}
		else
		{
			Melee();
			Attacking=true;
			PlayAnimMontage(AttackMontage, 1.0f);
		}
	}
	else
	{
		PunchWithWeapon();
	}
}

void AMS::StopFire()
{
	if (GearManager && GearManager->MasterWeapon && GearManager->MasterWeapon->Type != WeaponType::MS_Melee) {
		GearManager->MasterWeapon->StopFire();
	}
}

void AMS::Melee()
{
	// 令武器设置攻击

	// 让武器播放特有的攻击动作
	if (GearManager && GearManager->MasterWeapon && GearManager->MasterWeapon->Type >= WeaponType::MS_Melee) {
		GearManager->MasterWeapon->Melee();
	}
}

void AMS::PunchWithWeapon()
{
	
}

void AMS::RFire()
{

}

void AMS::SetDeath()
{
	if (bDied)
		return;

	bDied = true;// 角色死亡并令其控制瘫痪

	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();

	this->StopFire();
}

void AMS::PlayParriedMontage(AMsMeleeWeapon* Weapon, float val)
{
	if (bParried) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Error Parried Has opened")), false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Begin Parried")), false);
	}
	bParried = true;// 在此处打开被弹反的状态变量，在之后的动画通知中会关闭

	FWeaponMontage* RowMontage=MSMontageTable->FindRow<FWeaponMontage>(FName(TEXT("Parried")),TEXT("ParriedMontage"));
	if(RowMontage&&RowMontage->Montage)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Parried End")), false);
		PlayAnimMontage(RowMontage->Montage, RowMontage->Duration);
	}
}

void AMS::MoveForward(float Val)
{
	if (Val != 0.0f&&Moveable())
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void AMS::MoveRight(float Val)
{
	if (Val != 0.0f&&Moveable())
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void AMS::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//RotatorComponent->Turn(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	if(Rate!=0.0f&&Turnable())
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMS::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//RotatorComponent->LookUp(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	if(Rate!=0.0f&&Turnable())
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMS::Roll(float Rate)
{
	if(Rate!=0.0f&&Turnable())
	{
	//RotatorComponent->Roll(Rate * BaseRollRate * GetWorld()->GetDeltaSeconds());
	}
}

// Called every frame
void AMS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Called to bind functionality to input
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &AMS::TestTouch);
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMS::Melee);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMS::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMS::StopFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMS::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMS::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AMS::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AMS::LookUpAtRate);
	PlayerInputComponent->BindAxis("Roll", this, &AMS::Roll);

}

void AMS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMS, bParried, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AMS, bDied, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AMS, bStaggered, COND_SkipOwner);
}
