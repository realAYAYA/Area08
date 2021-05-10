// Fill out your copyright notice in the Description page of Project Settings.


#include "myComponents/MsHealthComponent.h"
#include "Net/UnrealNetwork.h"

#include"NumCalculation/Area08DamageType.h"
#include"Items/Bullet.h"

// Sets default values for this component's properties
UMsHealthComponent::UMsHealthComponent()
{
	// ...
	DefaultHeadHealth=60.0f;
	DefaultBodyHealth = 100.0f;
	DefaultArmHealth=60.0f;
	DefaultLegHealth=60.0f;

	//this->SetNetAddressable();//Make DSO components net addressable
	//this->SetIsReplicated(true);// Enable replication by default
	
}


void UMsHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
		return;
	
	const ABullet* BL = Cast<ABullet>(DamageCauser);
	if(BL&&BL->DamageType)
	{
		switch (BL->DamageType->GetHitReion())
		{
		case DamageRegion::Head:
			HeadHealth = HeadHealth >= Damage ? HeadHealth - Damage : 0.0f;
			break;
		case DamageRegion::Body:
			BodyHealth = BodyHealth >= Damage ? BodyHealth - Damage : 0.0f;
			break;
		case DamageRegion::RArm:
			RightArmHealth = RightArmHealth >= Damage ? RightArmHealth - Damage : 0.0f;
			break;
		case DamageRegion::LArm:
			LeftArmHealth = LeftArmHealth >= Damage ? LeftArmHealth - Damage : 0.0f;
			break;
		case DamageRegion::RLeg:
			RightLegHealth = RightLegHealth >= Damage ? RightLegHealth - Damage : 0.0f;
			break;
		case DamageRegion::LLeg:
			LeftLegHealth = LeftLegHealth >= Damage ? LeftLegHealth - Damage : 0.0f;
			break;
		}
	}
	else{	// 这块是处理其他伤害测试用的，全身掉血测试
		HeadHealth = HeadHealth >= Damage ? HeadHealth - Damage : 0.0f;
		BodyHealth = BodyHealth >= Damage ? BodyHealth - Damage : 0.0f;
		RightArmHealth = RightArmHealth >= Damage ? RightArmHealth - Damage : 0.0f;
		LeftArmHealth = LeftArmHealth >= Damage ? LeftArmHealth - Damage : 0.0f;
		RightLegHealth = RightLegHealth >= Damage ? RightLegHealth - Damage : 0.0f;
		LeftLegHealth = LeftLegHealth >= Damage ? LeftLegHealth - Damage : 0.0f;
	}
	
	if(BL)
	{
		OnHealthChanged.Broadcast(this, BodyHealth, Damage, BL->DamageType, InstigatedBy, DamageCauser);
	}
	else
	{	// 这块是处理其他伤害测试用的，全身掉血测试
		OnHealthChanged.Broadcast(this, BodyHealth, Damage, DamageType, InstigatedBy, DamageCauser);
	}
}

// Called when the game starts
void UMsHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitHealth();

	AActor* Owner = GetOwner();
	if (Owner) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UMsHealthComponent::HandleTakeDamage);
	}
}

void UMsHealthComponent::InitHealth()
{
	HeadHealth=DefaultHeadHealth;
	BodyHealth = DefaultBodyHealth;
	LeftArmHealth=RightArmHealth=DefaultArmHealth;
	LeftLegHealth=RightLegHealth=DefaultLegHealth;
}

void UMsHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UMsHealthComponent,HeadHealth,COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UMsHealthComponent,BodyHealth,COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UMsHealthComponent,LeftArmHealth,COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UMsHealthComponent,RightArmHealth,COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UMsHealthComponent,LeftLegHealth,COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UMsHealthComponent,RightLegHealth,COND_SkipOwner);

}
