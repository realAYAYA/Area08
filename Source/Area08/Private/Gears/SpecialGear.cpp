// Fill out your copyright notice in the Description page of Project Settings.


#include "Gears/SpecialGear.h"

ASpecialGear::ASpecialGear() {
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}