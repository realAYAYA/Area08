// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Area08DamageType.generated.h"

/**
 * 游戏内伤害类型
 */
UENUM()
enum class myDamageType : uint8
{
	Stab,// 突刺
	Slash,// 斩击
	Strike,// 打击
};

UENUM()
enum class DamageRegion : uint8
{
	// 伤害部位
	Head,
	Body,
	RArm,
	LArm,
	RLeg,
	LLeg,
	None,
};

UCLASS()
class AREA08_API UArea08DamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UArea08DamageType();

	void SetHitRegion(DamageRegion Val){this->HitRegion=Val;}
	DamageRegion GetHitReion()const{return this->HitRegion;}

	void SetDamageType(myDamageType Val){this->DamageType=Val;}
	myDamageType GetDamageType(){return  this->DamageType;}

	UPROPERTY(EditAnywhere)
	myDamageType DamageType;

	UPROPERTY(EditAnywhere)
	DamageRegion HitRegion;


};
