// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MsHealthComponent.generated.h"

// 添加自定义事件,用的时候放到顶上
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, class UMsHealthComponent*, HealthComp, float, Health, float, HeathDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS( ClassGroup=(MS), meta=(BlueprintSpawnableComponent) )
class AREA08_API UMsHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMsHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float DefaultHeadHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float DefaultBodyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float DefaultArmHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float DefaultLegHealth;


	UFUNCTION()
	void HandleTakeDamage(AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType, class AController* InstigatedBy, 
		AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;// 自定义事件

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
public:
	void InitHealth();

	float HeadHealth;
	float BodyHealth;
	float LeftArmHealth;
	float RightArmHealth;
	float LeftLegHealth;
	float RightLegHealth;

};
