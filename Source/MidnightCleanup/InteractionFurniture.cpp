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
#include "GeometryCollection\GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "Seunggi\FurnitureFieldSystemActor.h"
#include "ToolTipComponent.h"

AInteractionFurniture::AInteractionFurniture()
{
	ObjectType = EObjectType::ObjectType_Furniture;
	Box->SetBoxExtent(FVector::ZeroVector);
	
	Geometry = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	Geometry->SetupAttachment(RootComponent);
	Geometry->SetSimulatePhysics(false);
	if(StaticMesh)
	{
		StaticMesh->DestroyComponent();
	}
}

void AInteractionFurniture::BeginPlay()
{
	Super::BeginPlay();

	
	Box->SetBoxExtent(Geometry->GetLocalBounds().GetExtent());
	AddActorWorldTransform(FTransform(FVector(0, 0, Box->GetScaledBoxExtent().Z)));
	Geometry->SetRelativeLocation(FVector(0, 0, -Geometry->GetLocalBounds().GetExtent().Z));

	CurrentHp = MaxHp;
	//StaticMesh->SetVisibility(false);
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

void AInteractionFurniture::UpdateToolTip(APlayerCharacter* Player)
{
	FToolTipData* ToolTip = Player->ToolTip->GetToolTipByID(FName(TEXT("FurnitureBreak")));

	if (ToolTip && Player->CurrentToolTipData->Priority >= ToolTip->Priority)
	{
		Player->UpdateToolTip(ToolTip);
	}
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
	Geometry->SetRenderCustomDepth(state);
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
				
				S2A_BreakFurniture(500);
				CreateTrash(SpawnTrashNum);
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, this,&AInteractionFurniture::SelfDestroy, 1.0f, false, 1.0f);
			}
		}
	}
}

void AInteractionFurniture::S2A_BreakFurniture_Implementation(float Magnitude)
{
	Geometry->SetSimulatePhysics(true);
	Box->SetCollisionObjectType(ECC_WorldDynamic);
	Geometry->SetCollisionObjectType(ECC_WorldDynamic);
	if (FieldSystem == nullptr)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform Transform = FTransform(GetActorRotation(), GetActorLocation() + FVector(0, 0, 30), GetActorScale());

		FieldSystem = GetWorld()->SpawnActor<AFurnitureFieldSystemActor>(FieldSystemComponent, Transform, SpawnParam);
		if (FieldSystem)
		{
			FName name = FieldSystem->GetFName();
			UE_LOG(LogTemp, Warning, TEXT("nme : %s"), *name.ToString());
		}
	}
	
	FieldSystem->ApplyField(Magnitude);
	
}

void AInteractionFurniture::SelfDestroy()
{
	Destroy();
}

void AInteractionFurniture::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AInteractionFurniture, MaxHp);
	DOREPLIFETIME(AInteractionFurniture, CurrentHp);
}


