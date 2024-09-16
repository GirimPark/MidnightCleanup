// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanProgressMonitorBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include "Seunggi/InGameGM.h"	
#include "Seunggi/InGameGS.h"	
#include "Kismet/GameplayStatics.h"

void UCleanProgressMonitorBase::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("UCleanProgressMonitorBase::NativeConstruct"));

	FurnitureProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Furniture")));
	DecalProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Decal")));
	TrashProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Trash")));
	WoodProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Wood")));

	FurnitureCountBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("FurnitureCount")));
	DecalCountBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("DecalCount")));
	TrashCountBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("TrashCount")));
	WoodCountBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("WoodCount")));

	if (FurnitureCountBox)
	{
		FurnitureCountBox->SetText(FText::FromString(TEXT("InitText")));
	}

	if (DecalCountBox)
	{
		DecalCountBox->SetText(FText::FromString(TEXT("InitText")));
	}

	if (TrashCountBox)
	{
		TrashCountBox->SetText(FText::FromString(TEXT("InitText")));
	}

	if (WoodCountBox)
	{
		WoodCountBox->SetText(FText::FromString(TEXT("InitText")));
	}
}

void UCleanProgressMonitorBase::UpdateCount(int32 InFurniture, int32 InDecal, int32 InTrash, int32 InWood, int32 InitFurniture, int32 InitDecal, int32 InitTrash, int32 InitWood)
{
	if (FurnitureCountBox && FurnitureProgressBar)
	{
		FurnitureCountBox->SetText(FText::FromString(FString::FromInt(InFurniture)));
		FurnitureProgressBar->SetPercent((float)InFurniture / (float)InitFurniture);
	}

	if (DecalCountBox && DecalProgressBar)
	{
		DecalCountBox->SetText(FText::FromString(FString::FromInt(InDecal)));
		DecalProgressBar->SetPercent((float)InDecal / (float)InitDecal);
	}

	if (TrashCountBox && TrashProgressBar)
	{
		TrashCountBox->SetText(FText::FromString(FString::FromInt(InTrash)));
		TrashProgressBar->SetPercent((float)InTrash / (float)InitTrash);
	}

	if (WoodCountBox && WoodProgressBar)
	{
		WoodCountBox->SetText(FText::FromString(FString::FromInt(InWood)));
		WoodProgressBar->SetPercent((float)InWood / (float)InitWood);
	}
}
