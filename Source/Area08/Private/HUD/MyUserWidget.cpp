// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

#include "myComponents/MsHealthComponent.h"

void UMyUserWidget::Init()
{

		FLinearColor HudGreen = { 0.21f,1.0f,0.8f,1.0f };
		this->HudHead->SetColorAndOpacity(HudGreen);
		this->HudBody->SetColorAndOpacity(HudGreen);
		this->HudRA->SetColorAndOpacity(HudGreen);
		this->HudLA->SetColorAndOpacity(HudGreen);
		this->HudRL->SetColorAndOpacity(HudGreen);
		this->HudLL->SetColorAndOpacity(HudGreen);
}

void UMyUserWidget::UpdateHealth(const UMsHealthComponent* HealthComp)
{
	FLinearColor Green = { 0.21f,1.0f,0.8f,1.0f };
	FLinearColor Yellow = { 1.0f,0.41f,0.15f,1.0f };
	FLinearColor Red = { 1.0f,0,0,1.0f };
	if(HealthComp)
	{// 更新各个组件的血量显示
		float tempVal;
		if(this->HudHead)
		{
			tempVal=HealthComp->HeadHealth/HealthComp->DefaultHeadHealth;
			if(tempVal>=0.7f){this->HudHead->SetColorAndOpacity((Green));}
			else if(tempVal<0.7f&&tempVal>=0.4f){this->HudHead->SetColorAndOpacity((Yellow));}
			else{this->HudHead->SetColorAndOpacity((Red));}
		}
		{
			tempVal=HealthComp->BodyHealth/HealthComp->DefaultBodyHealth;
			if(tempVal>=0.7f){this->HudBody->SetColorAndOpacity((Green));}
			else if(tempVal<0.7f&&tempVal>=0.4f){this->HudBody->SetColorAndOpacity((Yellow));}
			else{this->HudBody->SetColorAndOpacity((Red));}
		}
		{
			tempVal=HealthComp->LeftArmHealth/HealthComp->DefaultArmHealth;
			if(tempVal>=0.7f){this->HudLA->SetColorAndOpacity((Green));}
			else if(tempVal<0.7f&&tempVal>=0.4f){this->HudLA->SetColorAndOpacity((Yellow));}
			else{this->HudLA->SetColorAndOpacity((Red));}
		}
		{
			tempVal=HealthComp->RightArmHealth/HealthComp->DefaultArmHealth;
			if(tempVal>=0.7f){this->HudRA->SetColorAndOpacity((Green));}
			else if(tempVal<0.7f&&tempVal>=0.4f){this->HudRA->SetColorAndOpacity((Yellow));}
			else{this->HudRA->SetColorAndOpacity((Red));}
		}
		{
			tempVal=HealthComp->LeftLegHealth/HealthComp->DefaultLegHealth;
			if(tempVal>=0.7f){this->HudLL->SetColorAndOpacity((Green));}
			else if(tempVal<0.7f&&tempVal>=0.4f){this->HudLL->SetColorAndOpacity((Yellow));}
			else{this->HudLL->SetColorAndOpacity((Red));}
		}
		{
			tempVal=HealthComp->RightLegHealth/HealthComp->DefaultLegHealth;
			if(tempVal>=0.7f){this->HudRL->SetColorAndOpacity((Green));}
			else if(tempVal<0.7f&&tempVal>=0.4f){this->HudRL->SetColorAndOpacity((Yellow));}
			else{this->HudRL->SetColorAndOpacity((Red));}
		}
	}
}

