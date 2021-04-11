// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Bullet.generated.h"

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FVector_NetQuantize HitPoint;
	UPROPERTY()
	FVector_NetQuantize HitRotation;
};

UCLASS()
class AREA08_API ABullet : public AItem
{
	GENERATED_BODY()

public:
	ABullet();
	

	UPROPERTY(EditAnywhere, Category = "Properties")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<class UArea08DamageType> BPDamageType;
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	class UArea08DamageType* DamageType;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float LifeTime;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UParticleSystem* HitFleshEffect;

	UParticleSystem* SelectEffect;// 最终要播放的特效

	UPROPERTY(EditAnywhere, Category = "Components")
	class USoundBase* HitSound;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent,
		AActor* HitActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void PlayHitEffects(EPhysicalSurface mySurfaceType,const FVector& HitPoint,const FRotator& HitRotation);
	void SetDamageType(const EPhysicalSurface& mySurfaceType);// 此方法将来使用了模块化骨骼网络后会删除

	UPROPERTY(ReplicatedUsing=OnRep_Hit)
	FHitScanTrace HitScanTrace;// 将子弹撞击的特效从服务器同步到客户端
	UFUNCTION()
	void OnRep_Hit();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
