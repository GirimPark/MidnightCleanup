// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSwitch.h"
#include "InteractionLightActor.h"
#include "Components\PointLightComponent.h"

AInteractionSwitch::AInteractionSwitch()
{
	Switch=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	Switch->SetupAttachment(StaticMesh);
}

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
		
		SetIntensity(5000);
		bIsLighting = true;
	}
}

void AInteractionSwitch::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AInteractionSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator ResultRotation = Switch->GetRelativeRotation();

	if (bIsLighting)
	{
		ResultRotation = FMath::RInterpTo(ResultRotation, TurnOnRotation, DeltaTime, 15);
		UE_LOG(LogTemp, Warning, TEXT("Turn On"));
	}
	else
	{
		ResultRotation = FMath::RInterpTo(ResultRotation, TurnOffRotation, DeltaTime, 15);
	}
	Switch->SetRelativeRotation(ResultRotation);
}

void AInteractionSwitch::SetIntensity_Implementation(float Value)
{
	Light->LightComponent->SetIntensity(Value);
	if (Value == 0)
	{
		bIsLighting = false;
	}
	else
	{
		bIsLighting = true;
	}
}
