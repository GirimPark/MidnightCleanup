// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "TitleWidgetBase.h"
#include "Kismet/GameplayStatics.h"

void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	if (TitleWidgetClass)
	{
		TitleWidget = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidget->AddToViewport();

		if (HasAuthority())
		{
			TitleWidget->ShowServerButton();
		}
		else
		{
			TitleWidget->HideServerButton();
		}
		
		FInputModeUIOnly InputMode;
		
		InputMode.SetWidgetToFocus(TitleWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		SetInputMode(InputMode);

		bShowMouseCursor = true;
	}
}

