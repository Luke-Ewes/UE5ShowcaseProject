// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE5COURSE_API AMainHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Main)
	TSubclassOf<class UMainOverlay> MainOverlayClass;

	UPROPERTY()
	UMainOverlay* MainOverlay;


public:
	FORCEINLINE UMainOverlay* GetMainOverlay() const { return MainOverlay; }
};
