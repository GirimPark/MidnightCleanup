// Fill out your copyright notice in the Description page of Project Settings.


#include "PC.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InGameWidgetBase.h"
#include "FurnitureProgressBase.h"
#include "RewardWidgetBase.h"
#include "RadialUIBase.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Seunggi/InGameGM.h"
#include "InteractionFurniture.h"

void APC::BeginPlay()
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

		if (InGameWidgetClass)
		{
			InGameWidget = CreateWidget<UInGameWidgetBase>(this, InGameWidgetClass);
			if (InGameWidget)
			{
				InGameWidget->AddToViewport();
				InGameWidget->RadialUIBox->SetVisibility(ESlateVisibility::Hidden);
				InGameWidget->FurnitureProgressBox->SetVisibility(ESlateVisibility::Hidden);
				InGameWidget->AimBox->SetVisibility(ESlateVisibility::Visible);
			}
		}

		if (RewardWidgetClass)
		{
			RewardWidget = CreateWidget<URewardWidgetBase>(this, RewardWidgetClass);
			if (RewardWidget)
			{
				RewardWidget->AddToViewport();
				RewardWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void APC::HideInventoryWidget(APlayerCharacter* InPlayer)
{
	if (InPlayer)
	{	
		UpdateInventoryByUI(InPlayer);
		InPlayer->SetInventory(0);
		InPlayer->C2S_SetUpdateInventoryState(false);
		InPlayer->C2S_UpdatePreviousInventoryIndex();
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	InGameWidget->RadialUIBox->SetVisibility(ESlateVisibility::Hidden);
}

void APC::ShowInventoryWidget(APlayerCharacter* InPlayer)
{
	if (InPlayer)
	{
		InGameWidget->UpdateInventoryByPlayer(InPlayer);
		InPlayer->C2S_SetUpdateInventoryState(true);
	}

	bShowMouseCursor = false;

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(InGameWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);

	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);

	InGameWidget->RadialUIBox->SetVisibility(ESlateVisibility::Visible);
}

void APC::HideAimWidget(APlayerCharacter* InPlayer)
{
	if (InGameWidget)
	{
		InGameWidget->AimBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APC::ShowAimWidget(APlayerCharacter* InPlayer)
{
	if (InGameWidget)
	{
		InGameWidget->AimBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void APC::HideRewardWidget()
{
	if (RewardWidget)
	{
		RewardWidget->SetVisibility(ESlateVisibility::Hidden);

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		/*AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->ChangeLevel(TEXT("L_InitGame"));
		}*/
	}
}

void APC::ShowRewardWidget(APlayerCharacter* InPlayer)
{
	if (RewardWidget)
	{
		RewardWidget->UpdateCleaningReward();

		RewardWidget->SetVisibility(ESlateVisibility::Visible);

		bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InGameWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		SetInputMode(InputMode);

		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);
	}
}

void APC::HideFurnitureProgressWidget()
{
	if (InGameWidget && InGameWidget->FurnitureProgressBox)
	{
		if (InGameWidget->FurnitureProgressBox->IsVisible())
		{
			InGameWidget->FurnitureProgressBox->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("Furniture Widget Hidden"));
		}
	}
}

void APC::ShowFurnitureProgressWidget()
{
	if (InGameWidget && InGameWidget->FurnitureProgressBox)
	{
		InGameWidget->FurnitureProgressBox->SetVisibility(ESlateVisibility::Visible);
	}
}

bool APC::IsEnableRewardWidget()
{
	if (RewardWidget && RewardWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APC::UpdateInventoryByUI(APlayerCharacter* InPlayer)
{
	if (InGameWidget)
	{
		InGameWidget->RadialUIBox->SetMousePos(InGameWidget->GetCurrentInventoryIndex());
	}

	if (InPlayer)
	{
		InPlayer->C2S_SetInventoryIndex(InGameWidget->GetCurrentInventoryIndex());
	}
}

void APC::UpdateFurnitureProgressBar(float InMaxHp, float InCurrentHp)
{
	if (InGameWidget && InGameWidget->FurnitureProgressBox)
	{
		InGameWidget->FurnitureProgressBox->UpdatePercent(InCurrentHp / InMaxHp);
	}
}

void APC::C2S_OpenInitGame_Implementation()
{
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->OpenInitGame();
	}
}
