// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionFaucet.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "../PlayerCharacter.h"
#include "../InteractionBucket.h"
#include "../ToolTipComponent.h"

AInteractionFaucet::AInteractionFaucet()
{
	Hologram = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HologramMesh"));
	Hologram->SetupAttachment(RootComponent);
	Box->SetSimulatePhysics(false);
}

void AInteractionFaucet::BeginPlay()
{
	Super::BeginPlay();
	Hologram->SetVisibility(false);
}

void AInteractionFaucet::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		if (Player->OwnedActor == nullptr)
		{
			return;
		}
		else
		{
			AInteractionBucket* Bucket = Cast<AInteractionBucket>(Player->OwnedActor);
			if (Bucket)
			{
				Player->DropObject();
				Bucket->bIsFilled = true;
				Bucket->Durability = Bucket->MaxDurability;
				S2A_FillBucket(Bucket);
			}
		}
	}
}

void AInteractionFaucet::DrawOutline(bool Draw)
{
	Hologram->SetVisibility(Draw);
}

void AInteractionFaucet::UpdateToolTip(APlayerCharacter* Player)
{
	FToolTipData* ToolTip = Player->ToolTip->GetToolTipByID(FName(TEXT("FillBucket")));

	if (ToolTip && Player->CurrentToolTipData->Priority >= ToolTip->Priority)
	{
		Player->UpdateToolTip(ToolTip);
	}
}

void AInteractionFaucet::S2A_FillBucket_Implementation(class AInteractionBucket* Bucket)
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld,true);

	Bucket->AttachToComponent(StaticMesh, AttachmentRules);
	Bucket->bIsFilled = true;
	Bucket->Durability = Bucket->MaxDurability;
	Bucket->DetachFromActor(DetachmentRules);

}
