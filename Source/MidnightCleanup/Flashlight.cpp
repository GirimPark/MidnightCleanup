// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Net/UnrealNetwork.h"

AFlashlight::AFlashlight()
{
	SetName(TEXT("Flashlight"));
	SetToolType(EToolType::ToolType_Flashlight);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetIntensity(0);
	SpotLight->SetupAttachment(StaticMesh);
}

void AFlashlight::BeginPlay()
{
	Super::BeginPlay();
}

void AFlashlight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlashlight, Intensity);
}

void AFlashlight::S2A_SetFlashLightOn_Implementation(bool State)
{
	UE_LOG(LogTemp, Warning, TEXT("C2A_SetFlashLightOn_Implementation"));

	if (State)
	{
		SpotLight->SetIntensity(Intensity);
		UE_LOG(LogTemp, Warning, TEXT("Flashlight On"))
	}
	else
	{
		SpotLight->SetIntensity(0);
		UE_LOG(LogTemp, Warning, TEXT("Flashlight Off"))
	}

	bIsActivate = State;
}

void AFlashlight::OnFlashlight(bool State)
{
	S2A_SetFlashLightOn(State);
}

