// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UMainOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UMainOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		const FString GoldString = FString::Printf(TEXT("%d"), Gold);
		GoldText->SetText(FText::FromString(GoldString));
	}
}

void UMainOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FString SoulsString = FString::Printf(TEXT("%d"), Souls);
		SoulsText->SetText(FText::FromString(SoulsString));
	}
}
