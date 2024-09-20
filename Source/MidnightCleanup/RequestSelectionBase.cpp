// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestSelectionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InitGamePC.h"
#include "GI.h"

void URequestSelectionBase::NativeConstruct()
{
	Super::NativeConstruct();

	StageBox1 = Cast<UButton>(GetWidgetFromName(TEXT("Stage1BTN_0")));
	if (StageBox1)
	{
		StageBox1->OnClicked.AddDynamic(this, &URequestSelectionBase::ClickedStageButton1);
	}

	StageBox2 = Cast<UButton>(GetWidgetFromName(TEXT("Stage1BTN_1")));
	if (StageBox2)
	{
		StageBox2->OnClicked.AddDynamic(this, &URequestSelectionBase::ClickedStageButton2);
	}

	UserIDBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("UserID")));
	if (UserIDBox)
	{
		UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			UserIDBox->SetText(FText::FromString(GI->GetUserID()));
		}
	}
}

void URequestSelectionBase::ClickedStageButton1()
{
	AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->C2S_OpenInGame();
	}
}

void URequestSelectionBase::ClickedStageButton2()
{
	AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->C2S_OpenInGame();
	}
}
