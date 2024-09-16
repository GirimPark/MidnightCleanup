// Fill out your copyright notice in the Description page of Project Settings.


#include "Sponge.h"
#include "Components/BoxComponent.h"
#include "OpacityDecal.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"

ASponge::ASponge()
{
	SetName(TEXT("Sponge"));
	SetToolType(EToolType::ToolType_Sponge);
	MaxDurability = 200;
	CleanDurability = 101;
	DirtyDurability = 0;
	Reduction = 1;
}

void ASponge::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ASponge::BoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ASponge::BoxEndOverlap);

	Durability = MaxDurability;
}

void ASponge::C2S_SetMaxDurability_Implementation()
{
	Durability = MaxDurability;

	S2A_SetSpongeDurability(Durability);
}

void ASponge::C2S_DecreaseDurability_Implementation()
{
	if (OverlappedDecal)
	{
		if (OverlappedDecal->CurrentOpacity  > 0.1)
		{
			if (Durability > 0)
			{
				float Temp = OverlappedDecal->CurrentOpacity - OverlappedDecal->Reduction;
				OverlappedDecal->S2A_SetOpacity(Temp);

				if (OverlappedDecal->CurrentOpacity > 0.1)
				{
					Durability -= Reduction;
				}
				else
				{
					OverlappedDecal->S2A_SetOpacity(0.f);
					S2A_DestoryOverlappedDecal();
				}
			}
							
			S2A_SetSpongeDurability(Durability);		
			S2A_SetSpongeMaterial(Durability);

			if (ObjectOwner)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
				ASponge* TPSSponge = Cast<ASponge>(Player->OwnedActor);
				if (TPSSponge)
				{
					TPSSponge->S2A_SetSpongeDurability(Durability);
					TPSSponge->S2A_SetSpongeMaterial(Durability);
				}
			}
		}
		else
		{
			S2A_DestoryOverlappedDecal();
		}
	}
}

void ASponge::S2A_SetSpongeDurability_Implementation(int32 InDurability)
{
	Durability = InDurability;
}

void ASponge::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("BoxBeginOverlap: %s"), *OtherActor->GetName());	
	AOpacityDecal* Decal = Cast<AOpacityDecal>(OtherActor);
	if (Decal)
	{
		if (ObjectOwner)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
			Player->bIsEraseStart = true;
			Player->C2S_SetSpongeOverlappedDecal(Decal);
		}
	}
}

void ASponge::BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACustomDecal* Decal = Cast<ACustomDecal>(OtherActor);
	if (Decal && Decal == OverlappedDecal)
	{
		if (ObjectOwner)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
			Player->C2S_SetSpongeOverlappedDecal(nullptr);
		}
	}
}

void ASponge::S2A_SetSpongeActivate_Implementation(bool State)
{
	bIsActivate = State;
}

void ASponge::S2A_DestoryOverlappedDecal_Implementation()
{
	if (OverlappedDecal)
	{
		OverlappedDecal->Destroy();
		OverlappedDecal = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Destroy OverlappedDecal"));
	}
}

void ASponge::S2A_SetOverlappedDecal_Implementation(AOpacityDecal* Decal)
{
	if (Decal)
	{
		OverlappedDecal = Decal;

		if (bIsActivate && !Decal->bWasInteractive)
		{
			float Temp = Decal->Reduction * 100;
			for (int i = 0; i < OpacityRatio / Temp; i++)
			{
				C2S_DecreaseDurability();
				//UE_LOG(LogTemp, Log, TEXT("C2S_SetDurability"));					
			}

			Decal->bWasInteractive = true;
		}
	}
}

void ASponge::S2A_CreateSpongeMaterial_Implementation()
{
	MI = StaticMesh->CreateDynamicMaterialInstance(0);
}

void ASponge::S2A_SetSpongeMaterial_Implementation(int32 InDurability)
{
	if (InDurability >= CleanDurability)
	{
		static FString Path = TEXT("/Game/Materials/interactive_object/Dirty_object_material/sponge/MI_Sponge_v1.MI_Sponge_v1");
		MI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *Path));
	}
	else if (InDurability <= DirtyDurability)
	{
		static FString Path = TEXT("/Game/Materials/interactive_object/Dirty_object_material/sponge/MI_Sponge_v3.MI_Sponge_v3");
		MI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *Path));
	}
	else
	{
		static FString Path = TEXT("/Game/Materials/interactive_object/Dirty_object_material/sponge/MI_Sponge_v2.MI_Sponge_v2");
		MI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *Path));
	}

	if (MI)
	{
		UE_LOG(LogTemp, Warning, TEXT("MI is Exist"))
			StaticMesh->SetMaterial(0, MI);
	}
}

void ASponge::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASponge, OpacityRatio);
	DOREPLIFETIME(ASponge, CleanDurability);
	DOREPLIFETIME(ASponge, DirtyDurability);
	DOREPLIFETIME(ASponge, MI);
	DOREPLIFETIME(ASponge, OverlappedDecal);
}
