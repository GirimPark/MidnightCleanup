// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionFurniture.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Axe.h"
#include "Net\UnrealNetwork.h"
#include "Seunggi\InteractionWoodenPlank.h"
#include "Seunggi/InGameGM.h"
#include "PC.h"
#include "Seunggi/InGameGS.h"

AInteractionFurniture::AInteractionFurniture()
{
	ObjectType = EObjectType::ObjectType_Furniture;
}

void AInteractionFurniture::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;

	if (bBreak)
	{
		AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->CountObject();
		}
	}
}

void AInteractionFurniture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractionFurniture::InterAction(APawn* Character)
{
}

void AInteractionFurniture::Destroyed()
{
	Super::Destroyed();

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->BreakFurnitureNum--;
		GS->OnRep_UpdateCount();
	}
}

void AInteractionFurniture::DrawOutline(bool state)
{
	StaticMesh->SetRenderCustomDepth(state);
}

void AInteractionFurniture::CreateTrash(uint8 TrashNum)
{
	for (int i = 0; i <TrashNum;i++)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Transform = FTransform(GetActorRotation(), GetActorLocation(), FVector(0.2, 0.2, 0.2));
		GetWorld()->SpawnActor<AInteractionWoodenPlank>(Trash, Transform, SpawnParam);
	}
}

void AInteractionFurniture::Axe_Hit(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		AAxe* Axe = Cast<AAxe>(Player->OwnedActor);
		if (Axe)
		{
			UE_LOG(LogTemp, Warning, TEXT("AXE"));

			CurrentHp -= 1;

			if (CurrentHp == 0)
			{
				CreateTrash(SpawnTrashNum);
				Destroy();
			}
		}
	}
}

void AInteractionFurniture::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AInteractionFurniture, MaxHp);
	DOREPLIFETIME(AInteractionFurniture, CurrentHp);
}


