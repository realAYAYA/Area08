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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPhysicsThrusterComponent* GravityManager;

	/**Runtime updated values. */
	/** Minimum view Pitch, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Pawn : Camera Settings")
	float CameraPitchMin;
    
	/** Maximum view Pitch, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Pawn : Camera Settings")
	float CameraPitchMax;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	/**Minimum Movement speed needed to orient the mesh toward the desired direction*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"), Category = "Custom Pawn : Mesh Rotation Settings", meta = (editcondition = "bRotateMeshOnlyWhenMoving"))
	float MinVelocityToRotateMesh;

	/** Controls how smooth the rotation is done , editable if InstantRotation is false  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Pawn : Mesh Rotation Settings", meta = (ClampMin = "0", UIMin = "0"), meta = (editcondition = "!bInstantRotation"))
	float RotationInterpSpeed;

	/**Character Mesh Initial Value. */
	FRotator MeshStartRotation;

	/** Current Forward Movement Direction*/
	FVector CurrentForwardDirection;
	/** Current Right Movement Direction*/
	FVector CurrentRightDirection;

	/**
	* Add input (affecting Pitch) to the SpringArm relative rotation.
	* The added value is the product of UpdateRate & ScaleValue.
	*/
    void AddCameraPitchInput(float UpdateRate = 1.0f, float ScaleValue = 0.0f);

	/**
	* Add input (affecting Yaw) to the SpringArm relative rotation.
	* The added value is the product of UpdateRate & ScaleValue.
	*/
    void AddCameraYawInput(float UpdateRate = 1.0f, float ScaleValue = 0.0f);

	virtual void UpdateMeshRotation(float DeltaTime){}// Run in Tick,Make Mesh follow the CameraSpringArm

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
};
