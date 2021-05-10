// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Area08PlayerControllerBase.h"
#include "Characters/MS.h"
#include "myComponents/MsAbilitySystemComponent.h"

void AArea08PlayerControllerBase::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	AMS* CharacterBase = Cast<AMS>(P);
	if (CharacterBase)
	{
		CharacterBase->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterBase, CharacterBase);
	}
}
