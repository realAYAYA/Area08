// Fill out your copyright notice in the Description page of Project Settings.
#include "Gears/MsGun.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"// Used by Replicated and GetLifetimeReplicatedProps()

#include "Items/Bullet.h"

AMsGun::AMsGun() {
	MuzzleSocketName = "Muzzle";

	FiringRate = 600;// Shoots per min

	TimeBetweenShoots = 60 / FiringRate;// Second per shoot

	// 联网代码
	{
		SetReplicates(true);// 武器联机可复制，由服务器发枪
		FireRep=0;
	}
}

void AMsGun::BeginPlay()
{
	
}

void AMsGun::OnFire()
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
	PlayEffects();
	
}

void AMsGun::StartFire()
{
	const float FirstDelay = FMath::Max(0.0f, LastFireTime + TimeBetweenShoots - GetWorld()->TimeSeconds);
	GetWorldTimerManager().SetTimer(TimeHandle_TimeBetweenShoots, this, &AMsGun::OnFire, TimeBetweenShoots, true, FirstDelay);
}

void AMsGun::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimeHandle_TimeBetweenShoots);
}

void AMsGun::PlayEffects()
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

void AMsGun::OnRep_Fire()
{
	PlayEffects();
}

void AMsGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMsGun, FireRep,COND_SkipOwner);
}

void AMsGun::SeverFire_Implementation()
{
	OnFire();
}

bool AMsGun::SeverFire_Validate()
{
	return true;
}

