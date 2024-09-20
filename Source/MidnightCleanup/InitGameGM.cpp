// Fill out your copyright notice in the Description page of Project Settings.


#include "InitGameGM.h"

void AInitGameGM::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bUseSeamlessTravel = true;
}

void AInitGameGM::OpenStage1()
{
	GetWorld()->ServerTravel(StageName1);
}

void AInitGameGM::OpenStage2()
{
	GetWorld()->ServerTravel(StageName2);
}

void AInitGameGM::OpenInGame()
{
	GetWorld()->ServerTravel(TEXT("L_InGame"));
}
