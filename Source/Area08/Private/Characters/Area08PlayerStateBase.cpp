// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Area08PlayerStateBase.h"

#include "myComponents/MsAbilitySystemComponent.h"

AArea08PlayerStateBase::AArea08PlayerStateBase()
{
	AbilitySystemComponent=CreateDefaultSubobject<UMsAbilitySystemComponent>(TEXT("MsAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}
