// Fill out your copyright notice in the Description page of Project Settings.


#include "myComponents/MsHealthComponent.h"

// Sets default values for this component's properties
UMsHealthComponent::UMsHealthComponent()
{
	// ...
	DefaultBodyHealth = 100.0f;
}


void UMsHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
		return;
	BodyHealth = BodyHealth >= Damage ? BodyHealth - Damage : 0.0f;
	OnHealthChanged.Broadcast(this, BodyHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

// Called when the game starts
void UMsHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitHealt();

	AActor* Owner = GetOwner();
	if (Owner) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UMsHealthComponent::HandleTakeDamage);
	}
}

void UMsHealthComponent::InitHealt()
{
	BodyHealth = DefaultBodyHealth;
}
