// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGS.h"
#include "InGameGM.h"
#include "Net\UnrealNetwork.h"
#include "../CleanProgressMonitor.h"
#include "../LimitTimeMonitor.h"
#include "../CleanProgressMonitorBase.h"
#include "Kismet/GameplayStatics.h"

void AInGameGS::OnRep_UpdateCount()
{
	TArray<AActor*> Monitors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACleanProgressMonitor::StaticClass(), Monitors);
	if (Monitors.Num() > 0)
	{
		ACleanProgressMonitor* Monitor = Cast<ACleanProgressMonitor>(Monitors[0]);
		if (Monitor)
		{
			Monitor->UpdateCount();
		}
	}
}

void AInGameGS::OnRep_UpdateTime()
{
	TArray<AActor*> Monitors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALimitTimeMonitor::StaticClass(), Monitors);
	if (Monitors.Num() > 0)
	{
		ALimitTimeMonitor* Monitor = Cast<ALimitTimeMonitor>(Monitors[0]);
		if (Monitor)
		{
			Monitor->UpdatePlayTime();
		}
	}
}

void AInGameGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameGS, TrashNum);
	DOREPLIFETIME(AInGameGS, SpawnDecalNum);
	DOREPLIFETIME(AInGameGS, OpacityDecalNum);
	DOREPLIFETIME(AInGameGS, WoodenPlankNum);
	DOREPLIFETIME(AInGameGS, BreakFurnitureNum);

	DOREPLIFETIME(AInGameGS, InitFurnitureCount);
	DOREPLIFETIME(AInGameGS, InitTextureDecalCount);
	DOREPLIFETIME(AInGameGS, InitOpacityDecalCount);
	DOREPLIFETIME(AInGameGS, InitTrashCount);
	DOREPLIFETIME(AInGameGS, InitWoodCount);

	DOREPLIFETIME(AInGameGS, CurrentPlayTime);
}

void AInGameGS::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayTime = PlayTime;
	OnRep_UpdateTime();
}

