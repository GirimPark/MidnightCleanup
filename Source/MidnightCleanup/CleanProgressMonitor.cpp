// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanProgressMonitor.h"
#include "Components/WidgetComponent.h"
#include "CleanProgressMonitorBase.h"
#include "Seunggi/InGameGM.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/EditableTextBox.h"
#include "Seunggi/InGameGS.h"

void ACleanProgressMonitor::BeginPlay()
{
	Super::BeginPlay();
	
	if (CleanAreaWidgetClass)
	{
		CleanAreaWidget = CreateWidget<UCleanProgressMonitorBase>(GetWorld(), CleanAreaWidgetClass);
		if (CleanAreaWidget)
		{
			Widget->SetWidget(CleanAreaWidget);
			Widget->InitWidget();

			UpdateCount();
		}
	}
}

void ACleanProgressMonitor::UpdateCount()
{
	if (CleanAreaWidget)
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS)
		{
			CleanAreaWidget->UpdateCount(
				GS->SpawnDecalNum, 
				GS->InitOpacityDecalCount - GS->OpacityDecalNum,
				GS->InitFurnitureCount - GS->BreakFurnitureNum,
				GS->InitTrashCount - GS->TrashNum,
				GS->WoodenPlankNum,
				0, 
				GS->InitTextureDecalCount, 
				GS->InitOpacityDecalCount, 
				GS->InitFurnitureCount,
				GS->InitTrashCount, 
				GS->InitWoodCount, 
				0);
		}
	}
}
