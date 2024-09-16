// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "InteractionPickUpObject.h"
#include "../PlayerCharacter.h"
#include "InGameGM.h"
#include "../InteractionBox.h"
#include "../InteractionBucket.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::InterAction(APawn* Character)
{
	if (SpawnObject)
	{
		if(SpawnObject->IsChildOf(AInteractionBox::StaticClass()))
		{
				UE_LOG(LogTemp, Warning, TEXT("Box num :%d"), AInteractionBox::ObjectNum)
			if(AInteractionBox::ObjectNum<LimitNum)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
				FActorSpawnParameters SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				FTransform Transform = GetActorTransform();
				AInteractionPickUpObject* Object = GetWorld()->SpawnActor<AInteractionPickUpObject>(SpawnObject, Transform, SpawnParam);
				if (Object && Player)
				{
					Object->InterAction(Player);
					Player->C2S_AddActorsToIgnore(Object);
					Player->C2S_SetHeldState(true);
					Player->C2S_SetOwnedActor(Object);
				}
				
			}
		}
		else
		{
			if (AInteractionBucket::ObjectNum < LimitNum)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
				FActorSpawnParameters SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				FTransform Transform = GetActorTransform();
				AInteractionPickUpObject* Object = GetWorld()->SpawnActor<AInteractionPickUpObject>(SpawnObject, Transform, SpawnParam);
				if (Object && Player)
				{
					Object->InterAction(Player);
					Player->C2S_AddActorsToIgnore(Object);
					Player->C2S_SetHeldState(true);
					Player->C2S_SetOwnedActor(Object);
				}
			}
		}
	}
}

void ASpawner::DrawOutline(bool Draw)
{
}

