// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"
#include "Math/UnitConversion.h"

void UHealthBarComponent::SetHealthPercentage(float HealthPercentage)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{ 
		HealthBarWidget->HealthBar->SetPercent(HealthPercentage);
	}
}
