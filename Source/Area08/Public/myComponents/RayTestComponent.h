// Fill out your copyright notice in the Description page of Project Settings.

/*
	使用方法：在人物中创建后，要对CameraComp变量进行初始化，射线配件需要捕捉摄像头的位置
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RayTestComponent.generated.h"


UCLASS( ClassGroup=(MS), meta=(BlueprintSpawnableComponent) )
class AREA08_API URayTestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URayTestComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Props)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Props)
	FColor Color;

	void Tracing();

	void SetCamera(class UCameraComponent* CameraComp) { CameraComponent = CameraComp; }

	void SetActivate(bool val) { this->Activated = val; }// Open/Close it

	void SetDebugLine(bool val) { this->DrawLine = val; }// Draw Line

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Props)
	bool Activated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Props)
	bool DrawLine;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UCameraComponent* CameraComponent;
	//FVector GetPawnViewLocation(UCameraComponent* CameraComp)const;
};
