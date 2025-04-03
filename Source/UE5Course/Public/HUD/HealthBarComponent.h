// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;
/**
 * 
 */
UCLASS()
class UE5COURSE_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	void SetHealthPercentage(float HealthPercentage);

private:

	UPROPERTY()
	UHealthBar* HealthBarWidget;
};
