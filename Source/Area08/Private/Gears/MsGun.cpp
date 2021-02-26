// Fill out your copyright notice in the Description page of Project Settings.


#include "Gears/MsGun.h"

#include "Items/Bullet.h"

AMsGun::AMsGun() {
	MuzzleSocketName = "Muzzle";

	FiringRate = 600;// Shoots per min

	TimeBetweenShoots = 60 / FiringRate;// Second per shoot
}

void AMsGun::BeginPlay()
{
}

void AMsGun::OnFire()
{
	if (Bullet != NULL)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		if (MeshComponent) {
			FTransform  SpawnLocation = MeshComponent->GetSocketTransform(MuzzleSocketName);
			ABullet* B = GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, ActorSpawnParams);
			if (B) {
				B->SetOwner(this->GetOwner());
			}
		}
	}
}

void AMsGun::StartFire()
{
	float FirstDelay = FMath::Max(0.0f, LastFireTime + TimeBetweenShoots - GetWorld()->TimeSeconds);
	GetWorldTimerManager().SetTimer(TimeHandle_TimeBetweenShoots, this, &AMsGun::OnFire, TimeBetweenShoots, true, FirstDelay);
}

void AMsGun::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimeHandle_TimeBetweenShoots);
}
