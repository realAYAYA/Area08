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
#include "Components/ProgressBar.h"
#include "Components/Image.h"


AMS::AMS() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDied = false;
	bParried = false;

	GearManager = CreateDefaultSubobject<UMSGearManager>(TEXT("GearManagement"));
	HealthManager = CreateDefaultSubobject<UMsHealthComponent>(TEXT("HealthComponent"));
	LineTracer= CreateDefaultSubobject<URayTestComponent>(TEXT("LineTracer"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PunchMontageObject(TEXT("AnimMontage'/Game/Combat/Melee/Attack1.Attack1'"));
	if (PunchMontageObject.Succeeded()) {
		AttackMontage = PunchMontageObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ParriedMontageObject(TEXT("AnimMontage'/Game/Combat/Melee/Parried.Parried'"));
	if (ParriedMontageObject.Succeeded()) {
		ParriedMontage = ParriedMontageObject.Object;
	}

}

void AMS::InitHUD()
{
	FLinearColor HudGreen = { 0.21f,1.0f,0.8f,1.0f };
	if (HUD) {
		HUD->HudHead->SetColorAndOpacity(HudGreen);
		HUD->HudBody->SetColorAndOpacity(HudGreen);
		HUD->HudRA->SetColorAndOpacity(HudGreen);
		HUD->HudLA->SetColorAndOpacity(HudGreen);
		HUD->HudRL->SetColorAndOpacity(HudGreen);
		HUD->HudLL->SetColorAndOpacity(HudGreen);
	}
}

FLinearColor AMS::UpdateHUD()
{
	FLinearColor HudGreen = { 0.21f,1.0f,0.8f,1.0f };
	FLinearColor HudYellow = { 1.0f,0.41f,0.15f,1.0f };
	FLinearColor HudRed = { 1.0f,0,0,1.0f };
	
	if (this->HealthManager && this->HealthManager->BodyHealth > 0) {
		float Val = this->HealthManager->BodyHealth;
		if (Val >= 66.0f) {
			return HudGreen;
		}
		else if (Val < 66.0f && Val >= 33.0f) {
			return HudYellow;
		}
		else{
			return HudRed;
		}
	}
	return HudRed;
}

void AMS::OnHealthChanged(UMsHealthComponent* OwnerHealthComp, float Health, float HeathDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bDied)
		return;

	this->UpdateHUD();

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
			if (HUD->HudBody) {
				HUD->HudBody->ColorAndOpacityDelegate.BindDynamic(this, &AMS::UpdateHUD);// 记得UFUNCTION()
			}
			InitHUD();
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Parried End")), false);
	PlayAnimMontage(ParriedMontage, 1.0f);
}

//void AMS::OnParried()
//{
//	if (ParriedMontage) {
//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString::SanitizeFloat(123), false);
//		PlayAnimMontage(ParriedMontage, 2.0f);
//	}
//}


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

}

void AMS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMS, bParried, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AMS, bDied, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AMS, bStaggered, COND_SkipOwner);
}
