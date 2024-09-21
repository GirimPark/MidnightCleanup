// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionKeypad.h"
#include "Kismet/GameplayStatics.h"
#include "PC.h"
#include "PlayerCharacter.h"
#include "Seunggi/InGameGM.h"
#include "ToolTipComponent.h"

void AInteractionKeypad::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		S2A_ShowRewardWidget(Player);
	}	
}

void AInteractionKeypad::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AInteractionKeypad::UpdateToolTip(APlayerCharacter* Player)
{
	FToolTipData* ToolTip = nullptr;

	if (TEXT("L_InitGame") == GetWorld()->GetMapName())
	{
		ToolTip = Player->ToolTip->GetToolTipByID(FName(TEXT("WorkStart")));
	}
	else
	{
		ToolTip = Player->ToolTip->GetToolTipByID(FName(TEXT("WorkFinish")));
	}
	
	if (ToolTip && Player->CurrentToolTipData->Priority >= ToolTip->Priority)
	{
		Player->UpdateToolTip(ToolTip);
		UE_LOG(LogTemp, Error, TEXT("AInteractionKeypad::UpdateToolTip"));
	}
}

void AInteractionKeypad::S2A_ShowRewardWidget_Implementation(APlayerCharacter* Player)
{
	Player->EnableKeypad();
}

