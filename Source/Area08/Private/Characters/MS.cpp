// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/MS.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"// 碰撞盒子 用于攻击判定
#include "PhysicalMaterials/PhysicalMaterial.h"// 用于判断物理材质
#include "UObject/ConstructorHelpers.h"// 用于通过资源引用来调用资源

#include "myComponents/MSGearManager.h"
#include "myComponents/MsHealthComponent.h"
#include "myComponents/RayTestComponent.h"
#include "myComponents/MsAbilitySystemComponent.h"
#include "Gears/MsWeapon.h"
#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"// Test HUD
#include "Characters/Area08PlayerControllerBase.h"
#include "Characters/Area08PlayerStateBase.h"

AMS::AMS() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// init MS status
	myStatus=MsStatus::Normal;
	myDriveMode=DriveMode::Walk;

	GearManager = CreateDefaultSubobject<UMSGearManager>(TEXT("GearManagement"));
	GearManager->SetIsReplicated(true);// Network
	HealthManager = CreateDefaultSubobject<UMsHealthComponent>(TEXT("HealthComponent"));
	HealthManager->SetIsReplicated(true);
	AbilitySystemComponent=CreateDefaultSubobject<UMsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	LineTracer= CreateDefaultSubobject<URayTestComponent>(TEXT("LineTracer"));
	
	AudioPlayComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPlayComp"));
	AudioPlayComponent->SetupAttachment(RootComponent);
	AudioPlayComponent->bAutoActivate = false;// Close Autoplay

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PunchMontageObject(TEXT("AnimMontage'/Game/Combat/Melee/Attack1.Attack1'"));
	if (PunchMontageObject.Succeeded()) {
		AttackMontage = PunchMontageObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> MontageTableObject(TEXT("DataTable'/Game/Test/DataTables/CharacterBaseMontages.CharacterBaseMontages'"));
	if (MontageTableObject.Succeeded()) {
		MSMontageTable = MontageTableObject.Object;
	}

	// 近战部分初始化
	MeleeBoxSocketName="Punch";
	MeleeBoxScale=FVector(1.5,1.5,1.5);// 拳头大小
	
	AttackBox = CreateDefaultSubobject<UBoxComponent>("AttackBoxComp");
	if (this->GetMesh()) {
		AttackBox->SetupAttachment(this->GetMesh(), "Attack");
	}
	AttackBox->SetCollisionProfileName("Weapon");
	AttackBox->SetNotifyRigidBodyCollision(true);// 生成撞击事件，不然无法触发碰撞
	AttackBox->SetBoxExtent(FVector(5));

	StaggeredBox = CreateDefaultSubobject<UBoxComponent>("BlockedBoxComp");
	if (this->GetMesh()) {
		StaggeredBox->SetupAttachment(this->GetMesh(), "Staggered");
	}
	StaggeredBox->SetCollisionProfileName("WeaponOther");
	StaggeredBox->SetNotifyRigidBodyCollision(true);
	StaggeredBox->SetBoxExtent(FVector(5));

	ParriedBox = CreateDefaultSubobject<UBoxComponent>("ParriedBoxComp");
	if (this->GetMesh()) {
		ParriedBox->SetupAttachment(this->GetMesh(), "Parried");
	}
	ParriedBox->SetCollisionProfileName("WeaponOther");
	ParriedBox->SetNotifyRigidBodyCollision(true);
	ParriedBox->SetBoxExtent(FVector(5));
}

bool AMS::Moveable()const
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

bool AMS::Runable()const
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

bool AMS::Turnable()const
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
	
	LineTracer->SetCamera(FirstPersonCameraComponent);

	// 加载一个测试用的HUD，后期删掉
	if (WidgetClass) {
		HUD = CreateWidget<UMyUserWidget>(GetWorld(), WidgetClass);
		if (HUD) {
			HUD->Init();
			HUD->AddToViewport();
		}
	}

	// 近战部分BeginPlay
	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMS::OnHit);// 绑定碰撞事件
	StaggeredBox->OnComponentBeginOverlap.AddDynamic(this, &AMS::OnParry);
	ParriedBox->OnComponentBeginOverlap.AddDynamic(this, &AMS::OnParry);
	// 如果碰撞开关放在构造函数里，有时会失效，可能是蓝图继承的关系
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaggeredBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParriedBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 绑定到武器，如果没有，就绑到拳头上
	UpdateCollisionBox();
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
	if (this->GearManager!=nullptr) {
		GearManager->StopMasterWeapon();
	}
}

void AMS::Melee()
{
	
}

void AMS::RFire()
{

}

void AMS::OnHit(UPrimitiveComponent* OverlappedComponent, 
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& Hit)// 武器命中要做的事情
{
	if (HitActor) {
		EPhysicalSurface tempSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Hit generate")), false);
		// switch (tempSurfaceType)
		// {
		// case MS_HEAD:
		// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Head hit.")), false);
		// 	break;
		// case MS_BODY:
		// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *FString(TEXT("Body hit.")), false);
		// 	break;
		// default:
		// 	break;
		// }

		if (AudioPlayComponent) {
			AudioPlayComponent->SetPitchMultiplier(FMath::RandRange(.5f, 4.f));

			//AudioPlayComponent->Play();
		}
	}
}

void AMS::OnParry(UPrimitiveComponent* OverlappedComponent, 
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)// 被弹反要做的事情
{
	
}

void AMS::OnStaggered(UPrimitiveComponent* OverlappedComponent, 
	AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)// 武器弹刀要做的事情
{	
	//bStaggered.Broadcast(this, 0.0f, 0.0f, 0.0f, InstigatedBy, DamageCauser);// 委托
}

void AMS::UpdateCollisionBox()
{
	AMsWeapon* W=this->GearManager->MasterWeapon;
	if(this->GearManager&&W)
	{
		AttackBox->AttachToComponent(W->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		W->MeleeBoxSocketName);// 先与武器绑定到位置，记得武器不在或者丢弃的时候解绑并重置盒子
		AttackBox->SetWorldScale3D(W->AttackBoxScale);// 设置大小

		StaggeredBox->AttachToComponent(W->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		W->MeleeBoxSocketName);
		StaggeredBox->SetWorldScale3D(W->StaggerBoxScale);

		ParriedBox->AttachToComponent(W->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		W->MeleeBoxSocketName);
		ParriedBox->SetWorldScale3D(W->ParryBoxScale);
	}else// 没有武器，碰撞盒绑定到拳头上
	{
		if(this->GetMesh())
		{
			AttackBox->AttachToComponent(this->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			this->MeleeBoxSocketName);// 先与武器绑定到位置，记得武器不在或者丢弃的时候解绑并重置盒子
			AttackBox->SetWorldScale3D(this->MeleeBoxScale);// 设置大小

			StaggeredBox->AttachToComponent(this->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			this->MeleeBoxSocketName);
			StaggeredBox->SetWorldScale3D(this->MeleeBoxScale);

			ParriedBox->AttachToComponent(this->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			this->MeleeBoxSocketName);
			ParriedBox->SetWorldScale3D(this->MeleeBoxScale);
		}
	}
}

void AMS::OnAttackEnableChanged(bool Enable)
{
	if (Enable) {
		AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMS::OnStaggeredEnableChanged(bool Enable)
{
	if (Enable) {
		StaggeredBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		StaggeredBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMS::OnParriedEnableChanged(bool Enable)
{
	if (Enable) {
		ParriedBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		ParriedBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

void AMS::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 对于玩家控制的Character且ASC位于Pawn, 我一般在服务端Pawn的PossessedBy()函数中初始化
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);

	// 对于玩家控制的Character且ASC位于PlayerState, 我一般在服务端Pawn的PossessedBy()函数中初始化
	// AArea08PlayerStateBase* PS = GetPlayerState<AArea08PlayerStateBase>();
	// if (PS)
	// {
	// 	// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
	// 	AbilitySystemComponent = Cast<UBuffManager>(PS->GetAbilitySystemComponent());
	//
	// 	// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
	// 	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	// }

	// 如果你遇到了错误消息LogAbilitySystem: Warning: Can't activate LocalOnly or LocalPredicted Ability %s when not local!
	// 那么就表明ASC没有在客户端中初始化.
	// ...
}

void AMS::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// 在客户端PlayerController的OnRep_PlayerState()函数中初始化, 这确保了PlayerState存在于客户端上.
	// AArea08PlayerStateBase* PS = GetPlayerState<AArea08PlayerStateBase>();
	// if (PS)
	// {
	// 	// Set the ASC for clients. Server does this in PossessedBy.
	// 	AbilitySystemComponent = Cast<UBuffManager>(PS->GetAbilitySystemComponent());
	//
	// 	// Init ASC Actor Info for clients. Server will init its `ASC` when it possesses a new Actor.
	// 	if(AbilitySystemComponent)
	// 	{
	// 		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	// 	}
	// }

	// 如果你遇到了错误消息LogAbilitySystem: Warning: Can't activate LocalOnly or LocalPredicted Ability %s when not local!
	// 那么就表明ASC没有在客户端中初始化.
	// ...
}

void AMS::PlayParriedMontage(AMsWeapon* Weapon, float val)
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
		// // add movement in that direction
		// const FVector UpDirection = GetActorUpVector();
		// const FVector CameraForward = this->GetSpringArm()->GetForwardVector();
		// const float Dot = FVector::DotProduct(UpDirection, CameraForward);
		//
		// if (FMath::Abs(Dot) < 1 - SMALL_NUMBER)
		// {
		// 	CurrentForwardDirection = FVector::VectorPlaneProject(CameraForward, GetActorUpVector());
		// }
	
		AddMovementInput(GetActorForwardVector(),Val);
	}
}

void AMS::MoveRight(float Val)
{
	if (Val != 0.0f&&Moveable())
	{
		// const FVector UpDirection = GetActorUpVector();
		// const FVector CameraRight = this->GetSpringArm()->GetRightVector();
		// const float Dot = FVector::DotProduct(UpDirection, CameraRight);
		//
		// if (FMath::Abs(Dot) < 1 - SMALL_NUMBER)
		// {
		// 	CurrentRightDirection = FVector::VectorPlaneProject(CameraRight, UpDirection);
		// }
	
		AddMovementInput(GetActorRightVector(),Val);
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
		//DodgeTime=DefaultDodgeTime;// 初始化闪避时间
		//myStatus=MsStatus::Dodging;
		FVector DodgeRotation = this->GetCharacterMovement()->Velocity.GetSafeNormal();
		this->GetCharacterMovement()->Velocity=DodgeRotation*20000.0f;
		// 播放闪避动作
		//GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &AMS::DodgeDrain, 0.5f, true);
	}
}

void AMS::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//RotatorComponent->Turn(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	if(Rate!=0.0f&&Turnable())
	{		
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		//AddCameraYawInput(1.0f,Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMS::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//RotatorComponent->LookUp(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	if(Rate!=0.0f&&Turnable())
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		//AddCameraPitchInput(1.0f,Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
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

	//Tick_Dodge(DeltaTime);
	//UpdateMeshRotation(DeltaTime);
}

void AMS::UpdateMeshRotation(float DeltaTime)
{
	//const float Speed = GetMovementComponent() != NULL ? GetMovementComponent()->Velocity.Size() : 0.0f;
	// if (Speed < MinVelocityToRotateMesh)
	// {
	// 	return;
	// }
	FRotator MeshRotation = GetMesh()->GetRelativeRotation();
	
	const FVector ProjectedVelocity = FVector::VectorPlaneProject(GetMovementComponent()->Velocity, GetActorUpVector());
	const FRotator Rot = FRotationMatrix::MakeFromXZ(GetTransform().InverseTransformVector(ProjectedVelocity), GetActorUpVector()).Rotator();
	
	//MeshRotation.Yaw = MeshStartRotation.Yaw + GetSpringArm()->GetRelativeRotation().Yaw;
	//MeshRotation.Roll = MeshStartRotation.Pitch + GetSpringArm()->GetRelativeRotation().Pitch;
	MeshRotation = MeshStartRotation + GetSpringArm()->GetRelativeRotation();

	GetMesh()->SetRelativeRotation(FMath::RInterpTo(GetMesh()->GetRelativeRotation(), MeshRotation, DeltaTime, RotationInterpSpeed*2));

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
	DOREPLIFETIME_CONDITION(AMS, myDriveMode, COND_SkipOwner);
}
