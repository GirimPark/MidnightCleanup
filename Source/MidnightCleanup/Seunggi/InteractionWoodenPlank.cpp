// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWoodenPlank.h"
#include "InGameGS.h"
#include "InGameGM.h"
#include "Kismet/GameplayStatics.h"

void AInteractionWoodenPlank::BeginPlay()
{
	Super::BeginPlay();
	bIsCleaning = false;
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}
}
