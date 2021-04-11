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
	myStatus=MsStatus::Normal;
	DefaultDodgeTime=5.0f;
	

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
	if(myStatus==MsStatus::Normal||myStatus==MsStatus::Moving)
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
	if(myStatus==MsStatus::Normal||myStatus==MsStatus::Moving)
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
	if (myStatus==MsStatus::bDied)
		return;
	// 更新HUD
	if(this->HUD&&this->HealthManager){this->HUD->UpdateHealth(this->HealthManager);}
	
	if(HasAuthority())
	{
		if (Health <= 0.0f && myStatus!=MsStatus::bDied)
		{
			myStatus=MsStatus::bDied;
			SetDeath();
		}
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

void AMS::TestTouch()
{
	// calculate delta for this frame from the rate information
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("TestTouching...")), false);
	LineTracer->Tracing();
}

void AMS::StartFire()
{
	if (this->GearManager!=nullptr)
	{
		this->GearManager->UseMasterWeapon();
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
	
}

void AMS::RFire()
{

}

void AMS::SetDeath()
{
	if (myStatus!=MsStatus::bDied)
		return;

	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();

	this->StopFire();
}

void AMS::PlayParriedMontage(AMsMeleeWeapon* Weapon, float val)
{
	myStatus=MsStatus::bDied;// 在此处打开被弹反的状态变量，在之后的动画通知中会关闭
	
	FWeaponMontage* RowMontage=MSMontageTable->FindRow<FWeaponMontage>(FName(TEXT("Parried")),TEXT("ParriedMontage"));
	if(RowMontage&&RowMontage->Montage)
	{
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

void AMS::SprintBegin()
{
	// if (Stamina >= StaminaCost) {
	// 	IsSprinting = true;
	{
		GetCharacterMovement()->MaxWalkSpeed = 2200;
		//GetWorldTimerManager().SetTimer(SprintTimerHandle, this, &AMS::SprintDrain, 0.5f, true);
	}
}

void AMS::SprintEnd()
{
	//IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	//GetWorldTimerManager().ClearTimer(SprintTimerHandle);
}

void AMS::Dodge()
{
	if(Moveable())
	{
		DodgeTime=DefaultDodgeTime;// 初始化闪避时间
		myStatus=MsStatus::Dodging;
		// 播放闪避动作
		//GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &AMS::DodgeDrain, 0.5f, true);
	}
}

void AMS::Tick_Dodge(float DeltaTime)
{
	// 在Tick循环中实现丝滑的闪避位移
	if(myStatus!=MsStatus::Dodging)
		return;
	if(DodgeTime>0)
	{
		DodgeTime=DodgeTime-DeltaTime>0?DodgeTime-DeltaTime:0;
		AddMovementInput(GetActorForwardVector(), 2);
		//AddActorLocalOffset(GetActorForwardVector()*1,true);
	}
	else
	{
		myStatus=MsStatus::Normal;
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
		AddControllerRollInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		GetFirstPersonCameraComponent();
	}
}

// Called every frame
void AMS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Tick_Dodge(DeltaTime);
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMS::SprintBegin);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMS::SprintEnd);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMS::Dodge);

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
	DOREPLIFETIME_CONDITION(AMS, myStatus, COND_SkipOwner);
}
