// Fill out your copyright notice in the Description page of Project Settings.


#include "OpacityDecal.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "Seunggi\InGameGM.h"
#include "Seunggi\InGameGS.h"
#include "Kismet\GameplayStatics.h"
#include "Components/DecalComponent.h"

AOpacityDecal::AOpacityDecal()
{
	CurrentOpacity = 1.0f;
	bWasInteractive = false;
	Reduction = 0.01f;
}

void AOpacityDecal::BeginPlay()
{
	Super::BeginPlay();
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}

	S2A_SetBoxExtent(Decal->DecalSize);
}

void AOpacityDecal::Destroyed()
{
	Super::Destroyed();

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OpacityDecalNum--;
		GS->OnRep_UpdateCount();
	}
}

void AOpacityDecal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOpacityDecal, CurrentOpacity);
	DOREPLIFETIME(AOpacityDecal, bWasInteractive);
	DOREPLIFETIME(AOpacityDecal, Reduction);
}

void AOpacityDecal::S2A_SetOpacity_Implementation(float Opacity)
{
	if (MI)
	{
		CurrentOpacity = Opacity;
		MI->SetScalarParameterValue(FName("Opacity"), CurrentOpacity);
	}
}



