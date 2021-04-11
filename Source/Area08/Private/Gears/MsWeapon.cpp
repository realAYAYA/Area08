// Fill out your copyright notice in the Description page of Project Settings.


#include "Gears/MsWeapon.h"
#include "Components/AudioComponent.h"
#include "UObject/ConstructorHelpers.h"// 用于通过资源引用来调用资源

AMsWeapon::AMsWeapon() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment(Root);

	AudioPlayComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPlayComp"));
	AudioPlayComponent->SetupAttachment(RootComponent);
	AudioPlayComponent->bAutoActivate = false;// Close Autoplay
}

void AMsWeapon::PlayEffect()
{
	
}
