// Fill out your copyright notice in the Description page of Project Settings.


#include "InitGamePC.h"
#include "LaptopWidgetBase.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InitGameGM.h"

void AInitGamePC::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem && DefaultIMC)
	{
		SubSystem->AddMappingContext(DefaultIMC, 0);
	}

	if (IsLocalPlayerController())
	{
		bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		if (LaptopWidgetClass)
		{
			LaptopWidget = CreateWidget<ULaptopWidgetBase>(this, LaptopWidgetClass);
			if (LaptopWidget)
			{
				LaptopWidget->AddToViewport();
				LaptopWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void AInitGamePC::ShowLaptopWidget()
{
	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	LaptopWidget->SetVisibility(ESlateVisibility::Visible);
}

void AInitGamePC::C2S_OpenInGame_Implementation()
{
	AInitGameGM* GM = Cast<AInitGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->OpenInGame();
	}
}
