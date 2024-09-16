// Fill out your copyright notice in the Description page of Project Settings.


#include "LimitTimeMonitor.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Seunggi/InGameGS.h"
#include "LimitTimeBase.h"

void ALimitTimeMonitor::BeginPlay()
{
	Super::BeginPlay();

	if (LimitTimeWidgetClass)
	{
		LimitTimeWidget = CreateWidget<ULimitTimeBase>(GetWorld(), LimitTimeWidgetClass);
		if (LimitTimeWidget)
		{
			Widget->SetWidget(LimitTimeWidget);
			Widget->InitWidget();

			UpdatePlayTime();
		}
	}
}

void ALimitTimeMonitor::UpdatePlayTime()
{
	if (LimitTimeWidget)
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS)
		{
			LimitTimeWidget->UpdatePlayTime(GS->CurrentPlayTime);
		}
	}
}
