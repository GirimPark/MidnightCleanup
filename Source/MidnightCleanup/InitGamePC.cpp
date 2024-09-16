// Fill out your copyright notice in the Description page of Project Settings.


#include "InitGamePC.h"
#include "LaptopWidgetBase.h"

void AInitGamePC::BeginPlay()
{
	Super::BeginPlay();

	if (LaptopWidgetClass)
	{
		if (IsLocalPlayerController())
		{
			LaptopWidget = CreateWidget<ULaptopWidgetBase>(this, LaptopWidgetClass);

			LaptopWidget->AddToViewport();

			bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			SetInputMode(InputMode);
		}
	}
}

void AInitGamePC::ChangeClientLevel()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	LaptopWidget->SetVisibility(ESlateVisibility::Hidden);
	SetInputMode(FInputModeGameOnly());

	ClientTravel(TEXT("/Game/Maps/L_TutorialTest.L_TutorialTest"), ETravelType::TRAVEL_Absolute);
}
