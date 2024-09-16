// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionMonitor.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractionMonitor::AInteractionMonitor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	if (Widget)
	{
		Widget->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AInteractionMonitor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractionMonitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


