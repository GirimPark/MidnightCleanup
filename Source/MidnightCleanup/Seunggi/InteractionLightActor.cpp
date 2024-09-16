// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionLightActor.h"
#include "Components\PointLightComponent.h"

// Sets default values
AInteractionLightActor::AInteractionLightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	LightComponent->SetupAttachment(StaticMesh);
}


