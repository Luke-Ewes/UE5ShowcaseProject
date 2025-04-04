// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/MainOverlay.h"

void AMainHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && MainOverlayClass)
		{
			MainOverlay = CreateWidget<UMainOverlay>(Controller, MainOverlayClass);
			MainOverlay->AddToViewport();
		}
	}
}
