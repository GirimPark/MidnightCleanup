// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicInteractionObject.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "../ToolTipComponent.h"
#include "Perception\AIPerceptionStimuliSourceComponent.h"

// Sets default values
ABasicInteractionObject::ABasicInteractionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SetReplicates(true);
	
	bReplicates = true;
	

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box ->SetCollisionProfileName(TEXT("Interactable"));
	RootComponent = Box;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	

	//StaticMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABasicInteractionObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicInteractionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

