// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionKeypad.h"
#include "Kismet/GameplayStatics.h"
#include "PC.h"
#include "PlayerCharacter.h"
#include "Seunggi/InGameGM.h"

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

void AInteractionKeypad::S2A_ShowRewardWidget_Implementation(APlayerCharacter* Player)
{
	Player->EnableKeypad();
}

