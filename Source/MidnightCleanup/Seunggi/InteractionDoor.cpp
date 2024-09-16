// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionDoor.h"

AInteractionDoor::AInteractionDoor()
{
	bIsOpened = false;
	ObjectType = EObjectType::ObjectType_Static;
	//StaticMesh->SetRelativeLocation(FVector(-110, 0, 0));
}

void AInteractionDoor::InterAction(APawn* Player)
{
	DoorMove();
}

void AInteractionDoor::DrawOutline(bool Draw)
{
}

void AInteractionDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector ResultPosition;
	if (bIsOpened)
	{
		ResultPosition = FMath::VInterpTo(StaticMesh->GetRelativeLocation(), OpenedPosition, GetWorld()->GetDeltaSeconds(), 7.0f);
	}
	else
	{
		ResultPosition = FMath::VInterpTo(StaticMesh->GetRelativeLocation(), ClosedPosition, GetWorld()->GetDeltaSeconds(), 7.0f);
		
	}
	StaticMesh->SetRelativeLocation(ResultPosition);
}

void AInteractionDoor::DoorMove_Implementation()
{
	FVector ResultPosition;
	if (bIsOpened)
	{
		bIsOpened = false;
	}
	else
	{
		bIsOpened = true;
		UE_LOG(LogTemp, Warning, TEXT("Open"));
	}
}

