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
			CleanAreaWidget->UpdateCount(GS->BreakFurnitureNum, GS->SpawnDecalNum + GS->OpacityDecalNum, GS->TrashNum, GS->WoodenPlankNum, GS->InitFurnitureCount, GS->InitOpacityDecalCount + GS->InitTextureDecalCount, GS->InitTrashCount, GS->InitWoodCount);
		}
	}
}
