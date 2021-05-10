// Fill out your copyright notice in the Description page of Project Settings.


#include "Gears/MsWeapon.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"// Used by Replicated and GetLifetimeReplicatedProps()

#include "Items/Bullet.h"
#include "UObject/ConstructorHelpers.h"// 用于通过资源引用来调用资源

AMsWeapon::AMsWeapon() {
	// 基本组件初始化
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment(Root);

	AudioPlayComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPlayComp"));
	AudioPlayComponent->SetupAttachment(RootComponent);
	AudioPlayComponent->bAutoActivate = false;// Close Autoplay

	// 枪械部分初始化
	MuzzleSocketName = "Muzzle";
	FiringRate = 600;// Shoots per min
	TimeBetweenShoots = 60 / FiringRate;// Second per shoot

	// 近战部分初始化
	MeleeBoxSocketName="MeleeBox";
	AttackBoxScale=FVector(1.0f,1.0f,2.0f);
	StaggerBoxScale=FVector(1.0f,1.0f,2.0f);
	ParryBoxScale=FVector(1.0f,1.0f,2.0f);

	// 联网
	SetReplicates(true);// 武器联机可复制，由服务器发枪
	FireRep=0;
}

void AMsWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(0.0f, LastFireTime + TimeBetweenShoots - GetWorld()->TimeSeconds);
	GetWorldTimerManager().SetTimer(TimeHandle_TimeBetweenShoots, this, &AMsWeapon::OnFire, TimeBetweenShoots, true, FirstDelay);
}

void AMsWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimeHandle_TimeBetweenShoots);
}

void AMsWeapon::OnFire()
{	
	if(GetLocalRole()<ROLE_Authority)
	{
		SeverFire();// 如果自己不是服务器，则申请服务器端开火，即客户端开火让服务端看到
	}

	// 检查弹夹：决定播放的特效和音效
	AudioPlayComponent->SetSound(FireSound);
	
	if (Bullet!= nullptr)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		if (MeshComponent) {
			FTransform SpawnLocation = MeshComponent->GetSocketTransform(MuzzleSocketName);
			ABullet* B = GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, ActorSpawnParams);
			if (B) {
				B->SetOwner(this->GetOwner());
			}
		}
	}
	
	// 服务器向客户端同步
	if(GetLocalRole()==ROLE_Authority)
	{
		FireRep=FireRep++%100;
	}
	/* 播放特效、音效、人物动作、镜头晃动或其他操作**/
	PlayFireEffects();
	
}

void AMsWeapon::PlayFireEffects()const
{
	if(MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect,MeshComponent,MuzzleSocketName);
	}
	if(AudioPlayComponent&&FireSound)
	{
		AudioPlayComponent->Play();
	}
}

// 联网部分
void AMsWeapon::OnRep_Fire()
{
	PlayFireEffects();
}

void AMsWeapon::SeverFire_Implementation()
{
	OnFire();
}

bool AMsWeapon::SeverFire_Validate()
{
	return true;
}

void AMsWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMsWeapon, FireRep,COND_SkipOwner);
}

void AMsWeapon::BeginPlay()
{
	
}