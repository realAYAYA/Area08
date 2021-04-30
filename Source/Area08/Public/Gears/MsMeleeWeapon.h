// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gears/MsWeapon.h"
#include "MsMeleeWeapon.generated.h"

/**
 * 机甲的近战武器实现:在初始化时，要记得将MS的弹刀，和弹反事件与该武器实例进行委托
 */
// 自定义事件：被弹反
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnParriedSignature,class AMsMeleeWeapon*,MeleeWeapon,float,Val);
// 自定义事件：被弹刀

UCLASS()
class AREA08_API AMsMeleeWeapon : public AMsWeapon
{
	GENERATED_BODY()
	
public:
	AMsMeleeWeapon();

	/** 近战攻击部分，可被弹刀，被招架 判定开启*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnParry(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnStaggered(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void OnAttackEnableChanged(bool Enable)override;
	void OnStaggeredEnableChanged(bool Enable)override;
	void OnParriedEnableChanged(bool Enable)override;

	virtual void MeleeBreak()override;

	virtual void Melee()override;

	UPROPERTY(BlueprintAssignable, Category = "MeleeEvents")
	FOnParriedSignature OnParriedChanged;

	/*UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaggeredSignature bStaggered;*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/* 武器特效*/
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	//UParticleSystem* MuzzleEffect;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	//UParticleSystem* HitEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = "true"))
	USoundBase* UnKnowSound;

	/** 近战攻击部分，可被弹刀，被招架 判定开启*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* AttackBox;// 攻击判断盒子

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* StaggeredBox;// 被弹刀判定盒子

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* ParriedBox;// 被招架判, 弹刀定盒子

	/* 人物指定武器动画，在特定条件下使用人物播放*/
	// 攻击动画 // 被弹反动画// 被弹刀动画

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	USoundBase* HitSound;
};
