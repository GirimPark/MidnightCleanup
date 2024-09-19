// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanProgressMonitorBase.h"
#include "Seunggi/InGameGM.h"	
#include "Seunggi/InGameGS.h"	
#include "Kismet/GameplayStatics.h"
#include "CleanProgressBarBase.h"

void UCleanProgressMonitorBase::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("UCleanProgressMonitorBase::NativeConstruct"));

	MopDecalProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_MopDecal")));
	SpongeDecalProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_SpongeDecal")));
	FurnitureProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Furniture")));
	TrashProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Trash")));
	WoodProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Wood")));
	PropsProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Props")));
}

void UCleanProgressMonitorBase::UpdateCount(int32 InMopDecal, int32 InSpongeDecal, int32 InFurniture, int32 InTrash, int32 InWood, int32 InProps, int32 InitMopDecal, int32 InitSpongeDecal, int32 InitFurniture, int32 InitTrash, int32 InitWood, int32 InitProps)
{
	if (MopDecalProgressBox)
	{
		MopDecalProgressBox->UpdateCount(InMopDecal, InitMopDecal);
	}

	if (SpongeDecalProgressBox)
	{
		SpongeDecalProgressBox->UpdateCount(InSpongeDecal, InitSpongeDecal);
	}

	if (FurnitureProgressBox)
	{
		FurnitureProgressBox->UpdateCount(InFurniture, InitFurniture);
	}

	if (TrashProgressBox)
	{
		TrashProgressBox->UpdateCount(InTrash, InitTrash);
	}

	if (WoodProgressBox)
	{
		WoodProgressBox->UpdateCount(InWood, InitWood);
	}

	if (PropsProgressBox)
	{
		PropsProgressBox->UpdateCount(InProps, InitProps);
	}
}
