// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTool.h"
#include "Flashlight.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Mop.h"
#include "Sponge.h"

// Sets default values
ABasicTool::ABasicTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpObjectType = EPickUpObjectType::PickUpObjectType_Tool;	

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABasicTool::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABasicTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString ABasicTool::GetName()
{
	return Name;
}

void ABasicTool::SetName(FString InName)
{
	Name = InName;
}

void ABasicTool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicTool, MaxDurability);
	DOREPLIFETIME(ABasicTool, bIsActivate);
	DOREPLIFETIME(ABasicTool, Reduction);
}

void ABasicTool::AttachPreviousSocket(APlayerCharacter* Player)
{
	S2A_AttachTool(Player);
}

void ABasicTool::DetachPreviousSocket(APlayerCharacter* Player)
{
	S2A_DetachTool(Player);
}

EToolType ABasicTool::GetToolType()
{
	return ToolType;
}

void ABasicTool::SetToolType(EToolType InToolType)
{
	ToolType = InToolType;
}

void ABasicTool::VisibleObject(bool State)
{

	S2A_VisibleTool(State);
}

void ABasicTool::PickUpObject(APawn* Character)
{
	UE_LOG(LogTemp,Warning,TEXT("Pick up tool"))
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		);

		switch (GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			Box->SetSimulatePhysics(false);
			Player->C2S_SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Mop"));	

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Mop"));
			}
		}
		break;
		case EToolType::ToolType_Axe:
		{
			Box->SetSimulatePhysics(false);
			Player->C2S_SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Axe"));

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Axe"));
			}
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			Box->SetSimulatePhysics(false);
			Player->C2S_SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Sponge"));	

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Sponge"));
			}
		}
		break;
		case EToolType::ToolType_Flashlight:
		{
			Box->SetSimulatePhysics(false);
			Player->C2S_SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Flashlight"));

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Flashlight"));
			}
		}
		break;
		}

		if (Player->IsLocallyControlled())
		{
			if (GetToolType() == EToolType::ToolType_Mop)
			{
				UE_LOG(LogTemp, Warning, TEXT("FPS Mesh visible"));
				AMop* Mop = Cast<AMop>(this);
				if (Mop)
				{
					Mop->SkeletalMesh->SetVisibility(false);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FPS Mesh visible"));
				StaticMesh->SetVisibility(false);
			}
			
		}
		else
		{
			if (GetToolType() == EToolType::ToolType_Mop)
			{
				UE_LOG(LogTemp, Warning, TEXT("FPS Mesh visible"));
				AMop* Mop = Cast<AMop>(this);
				if (Mop)
				{
					Mop->SkeletalMesh->SetVisibility(true);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FPS Mesh visible"));
				StaticMesh->SetVisibility(true);
			}
		}
	}
}

void ABasicTool::DropProcess(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		FDetachmentTransformRules DetachmentRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			false
		);

		switch (GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			Player->C2S_SetToolHeldState(this, false);
		}
		break;
		case EToolType::ToolType_Axe:
		{
			Player->C2S_SetToolHeldState(this, false);
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			DropSponge(Player);
			Player->C2S_SetToolHeldState(this, false);		
		}
		break;
		case EToolType::ToolType_Flashlight:
		{
			Player->C2S_SetToolHeldState(this, false);
		}
		break;
		}

		DetachFromActor(DetachmentRules);
		Box->SetSimulatePhysics(true);

		if (Player->FPSOwnedActor)
		{
			Player->FPSOwnedActor->Destroy();
			Player->FPSOwnedActor = nullptr;
			StaticMesh->SetVisibility(true);
		}
	}
}

void ABasicTool::PickUpTool(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if(Player)
	{
		
	}
}


void ABasicTool::S2A_VisibleTool_Implementation(bool State)
{
	if (StaticMesh)
	{
		StaticMesh->SetVisibility(State);
	}
}

void ABasicTool::S2A_AttachTool_Implementation(APlayerCharacter* Player)
{
	if (Player)
	{
		DropProcess(Player);
		Box->SetSimulatePhysics(false);

		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			true
		);
		
		AttachToComponent(Player->GetMesh(), AttachmentRules, FName("PreviousTool"));
	}
}

void ABasicTool::S2A_DetachTool_Implementation(APlayerCharacter* Player)
{
	if (Player)
	{
		FDetachmentTransformRules DetachmentRules(
				EDetachmentRule::KeepWorld,
				EDetachmentRule::KeepWorld,
				EDetachmentRule::KeepWorld,
				false
			);

		DetachFromActor(DetachmentRules);
		
		//PickUpTool(Player);
	}
}

void ABasicTool::PickUpFPSSponge(APlayerCharacter* Player)
{
	if (Player && Player->FPSOwnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickUpSponge"));

		Player->FPSOwnedActor->Box->SetSimulatePhysics(true);
		Player->PhysicsHandle->GrabComponentAtLocationWithRotation(Player->FPSOwnedActor->Box, NAME_None, Player->FPSOwnedActor->GetActorLocation(), Player->FPSOwnedActor->GetActorRotation());
	}
}

void ABasicTool::DropSponge(APlayerCharacter* Player)
{
	if (Player)
	{
		Player->PhysicsHandle->ReleaseComponent();		
		Player->OwnedActor = nullptr;
	}
}


