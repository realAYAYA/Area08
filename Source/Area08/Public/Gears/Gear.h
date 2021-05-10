// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gear.generated.h"

UCLASS()
class AREA08_API AGear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGear();

	class AMS* Holder;// 保存武器的持有者，方便在武器攻击的时调用人物播放动画
	void SetHolder(AMS* Val){this->Holder=Val;}

	virtual void Activate() {};
	virtual void Breaken() {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	int ID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	FName Name;

	/* Parameter compensation*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	float weigth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	int Engine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	float Power;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	float TurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	float LookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParametersCompensation)
	float RollRate;
};
