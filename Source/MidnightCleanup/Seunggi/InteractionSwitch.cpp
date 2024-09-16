// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSwitch.h"
#include "InteractionLightActor.h"
#include "Components\PointLightComponent.h"

void AInteractionSwitch::InterAction(APawn* Character)
{
	if (Light == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Light is not connected"));
		return;
	}
	if (Light->LightComponent->Intensity != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Turn Off"));
		SetIntensity(0);
		bIsLighting = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Turn On"));
		SetIntensity(5000);
		bIsLighting = true;
	}
}

void AInteractionSwitch::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AInteractionSwitch::SetIntensity_Implementation(float Value)
{
	Light->LightComponent->SetIntensity(Value);
}
