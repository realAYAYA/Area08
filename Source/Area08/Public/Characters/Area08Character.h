// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Area08Character.generated.h"

UCLASS()
class AArea08Character : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AArea08Character();

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Force, meta = (AllowPrivateAccess = "true"))
	class UPhysicsThrusterComponent* GravityManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn, look up/down, roll at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Roll(float Rate);

public:
	/** Base turn||look up/down||roll rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseParameters)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseParameters)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseParameters)
	float BaseRollRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseParameters)
	float BaseMoveForwardSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseParameters)
	float BaseMoveBackSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseParameters)
	float BaseMoveJetSpeed;


	bool bMovementable;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
