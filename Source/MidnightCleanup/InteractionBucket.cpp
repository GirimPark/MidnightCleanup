// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionBucket.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "Mop.h"
#include "Sponge.h"
#include "Seunggi\InGameGM.h"

uint8 AInteractionBucket::ObjectNum = 0;

AInteractionBucket::AInteractionBucket()
{
	PickUpObjectType = EPickUpObjectType::PickUpObjectType_Bucket;
	BucketState = EBucketState::BucketState_Clean;
	Durability = MaxDurability;
}

void AInteractionBucket::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionBucket, Durability);
}

void AInteractionBucket::BeginPlay()
{
	Super::BeginPlay();
	Durability = 0;
	bIsFilled = false;
	if(HasAuthority())
	{
		ObjectNum++;
	}
}

void AInteractionBucket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ObjectNum = 0;
}

void AInteractionBucket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractionBucket::Destroyed()
{
	Super::Destroyed();

	ObjectNum--;
}

void AInteractionBucket::Cleaning(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		ABasicTool* Tool = Cast<ABasicTool>(Player->OwnedActor);
		if (Tool && bIsFilled)
		{
			if (Tool->ToolType == EToolType::ToolType_Mop)
			{
				AMop* Mop = Cast<AMop>(Tool);
				AMop* FPSMop = Cast<AMop>(Player->FPSOwnedActor);
				if (Mop && FPSMop)
				{
					if (Durability > 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("Set Durability"))
							Durability -= (Mop->MaxDurability - Mop->Durability);
						Mop->Durability = Mop->MaxDurability;
						Mop->S2A_SetMopDurability(Mop->MaxDurability);
						Mop->S2A_SetMopMaterial(Mop->MaxDurability);

						FPSMop->S2A_SetMopDurability(Mop->MaxDurability);
						FPSMop->S2A_SetMopMaterial(Mop->MaxDurability);

						if (Durability < 0)
						{
							Durability = 0;
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Set Durability 0"));
						Mop->Durability = 0;
						Mop->S2A_SetMopDurability(0);
						Mop->S2A_SetMopMaterial(0);

						FPSMop->S2A_SetMopDurability(0);
						FPSMop->S2A_SetMopMaterial(0);
					}
				}
			}
			else if (Tool->ToolType == EToolType::ToolType_Sponge)
			{
				ASponge* Sponge = Cast<ASponge>(Tool);
				ASponge* FPSSponge = Cast<ASponge>(Player->FPSOwnedActor);

				if (FPSSponge && Sponge)
				{
					if (Durability > 0)
					{
						Durability -= (FPSSponge->MaxDurability - FPSSponge->Durability);

						FPSSponge->Durability = FPSSponge->MaxDurability;
						FPSSponge->S2A_SetSpongeDurability(FPSSponge->MaxDurability);
						FPSSponge->S2A_SetSpongeMaterial(FPSSponge->MaxDurability);

						Sponge->S2A_SetSpongeDurability(FPSSponge->MaxDurability);
						Sponge->S2A_SetSpongeMaterial(FPSSponge->MaxDurability);

						if (Durability < 0)
						{
							Durability = 0;
						}
					}
					else
					{
						FPSSponge->Durability = 0;
						FPSSponge->S2A_SetSpongeDurability(0);
						FPSSponge->S2A_SetSpongeMaterial(0);

						Sponge->S2A_SetSpongeDurability(0);
						Sponge->S2A_SetSpongeMaterial(0);
					}

				}
			}
		}
	}
}


