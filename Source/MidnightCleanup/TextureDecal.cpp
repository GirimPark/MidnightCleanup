// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureDecal.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Seunggi/InGameGM.h"
#include "Seunggi/InGameGS.h"

ATextureDecal::ATextureDecal()
{
	Durability = 3;
}

void ATextureDecal::BeginPlay()
{
	Super::BeginPlay();

	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}

	if (MI)
	{
		MI->SetVectorParameterValue(FName("BaseColor"), BaseColor);
		MI->SetScalarParameterValue(FName("Contrast"), Contrast);
		MI->SetScalarParameterValue(FName("Roughness"), Roughness);
		
		switch (Durability)
		{
		case 1:
		{
			MI->SetTextureParameterValue(FName("Alpha"), AlphaTexture_3);
		}
		break;
		case 2:
		{
			MI->SetTextureParameterValue(FName("Alpha"), AlphaTexture_2);
		}
		break;
		case 3:
		{
			MI->SetTextureParameterValue(FName("Alpha"), AlphaTexture_1);
		}
		break;
		}

		MI->SetScalarParameterValue(FName("Opacity"), 1.0f);
	}

	S2A_SetBoxExtent(Decal->DecalSize);
}

void ATextureDecal::Destroyed()
{
	Super::Destroyed();
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->SpawnDecalNum--;
		GS->OnRep_UpdateCount();
	}
}

void ATextureDecal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATextureDecal, OpacityLevel_1);
	DOREPLIFETIME(ATextureDecal, OpacityLevel_2);
	DOREPLIFETIME(ATextureDecal, Contrast);
	DOREPLIFETIME(ATextureDecal, Roughness);
	DOREPLIFETIME(ATextureDecal, BaseColor);
}

void ATextureDecal::S2A_SetAlphaTexture_Implementation(UTexture* Texture)
{
	if (MI)
	{
		MI->SetTextureParameterValue(FName("Alpha"), Texture);
	}
}

void ATextureDecal::S2A_SetSize_Implementation(FVector InSize)
{
	if (Decal)
	{
		Decal->DecalSize = InSize;
	}
}

void ATextureDecal::C2S_SetAlphaTextureAndOpacity_Implementation()
{
	if (Durability > 0)
	{
		Durability -= 1;
	}

	switch (Durability)
	{
	case 0:
	{
		S2A_Destroy();
	}
	break;
	case 1:
	{
		if (AlphaTexture_3)
		{
			S2A_SetAlphaTexture(AlphaTexture_3);
			S2A_SetOpacityLevel(OpacityLevel_2);
		}		
	}
	break;
	case 2:
	{
		if (AlphaTexture_2)
		{
			S2A_SetAlphaTexture(AlphaTexture_2);
			S2A_SetOpacityLevel(OpacityLevel_1);
		}
	}
	break;
	}

}

void ATextureDecal::S2A_SetOpacityLevel_Implementation(float InOpacity)
{
	if (MI)
	{
		MI->SetScalarParameterValue(FName("Opacity"), InOpacity);
	}
}

void ATextureDecal::S2A_Destroy_Implementation()
{
	if (MI)
	{
		MI->SetScalarParameterValue(FName("Opacity"), 0.f);
	}

	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->SpawnDecal.Remove(this);
	}

	Destroy();
}
