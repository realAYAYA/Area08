// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class AREA08_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	FName Name;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int ID;
protected:

public:	

};
