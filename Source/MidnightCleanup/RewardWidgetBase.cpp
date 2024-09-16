// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "GI.h"
#include "PC.h"
#include "Seunggi/InGameGS.h"

void URewardWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UserIDBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("UserID")));
	if (UserIDBox)
	{
		UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			UserIDBox->SetText(FText::FromString(GI->GetUserID()));
		}
	}

	ReturnBox = Cast<UButton>(GetWidgetFromName(TEXT("ReturnBTN")));
	if (ReturnBox)
	{
		ReturnBox->OnClicked.AddDynamic(this, &URewardWidgetBase::ClickedReturnButton);
	}

	CleaningRewardBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("CleaningReward")));

	StarLayoutBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("StarLayout")));
	TotalRewardBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalReward")));
}

void URewardWidgetBase::ClickedReturnButton()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->IsEnableRewardWidget())
	{
		PC->HideRewardWidget();
		PC->C2S_OpenInitGame();
	}
}

void URewardWidgetBase::UpdateCleaningReward()
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		int32 DecalAndTrash = GS->TrashNum + GS->SpawnDecalNum + GS->OpacityDecalNum;

		UE_LOG(LogTemp, Warning, TEXT("[Widget] DecalAndTrash : %d"), DecalAndTrash);

		if (DecalAndTrash <= 10)
		{
			CleaningReward += 100;
		}

		if (6 <= DecalAndTrash && DecalAndTrash <= 10)
		{
			StarNum = 1;
			UpdateStarIcon();
		}
		else if (1 <= DecalAndTrash && DecalAndTrash <= 5)
		{
			StarNum = 2;
			UpdateStarIcon();
		}
		else if (DecalAndTrash == 0)
		{
			CleaningReward += 30;

			StarNum = 5;
			UpdateStarIcon();
		}
		
		if (DecalAndTrash > 10)
		{
			CleaningReward -= 100;

			StarNum = 0;
			UpdateStarIcon();
		}

		if (CleaningRewardBox)
		{
			CleaningRewardBox->SetText(FText::FromString(FString::FromInt(CleaningReward)));
		}
	}
	else
	{
		if (CleaningRewardBox)
		{
			CleaningRewardBox->SetText(FText::FromString(TEXT("GameState is not Exist")));
			UpdateTotalReward();
		}
	}
}

void URewardWidgetBase::UpdateStarIcon()
{
	if (StarLayoutBox)
	{
		TArray<UWidget*> TempArr = StarLayoutBox->GetAllChildren();
		for (int i = 0; i < StarNum; i++)
		{
			UImage* Image = Cast<UImage>(TempArr[i]);
			if (Image)
			{
				FSlateBrush Brush = Image->GetBrush();
				if (StarIconTexture)
				{
					Brush.SetResourceObject(StarIconTexture);
					Image->SetBrush(Brush);
				}
			}
		}

		for (int j = StarNum; j < TempArr.Num(); j++)
		{
			UImage* Image = Cast<UImage>(TempArr[j]);
			if (Image)
			{
				FSlateBrush Brush = Image->GetBrush();
				if (StarBackgroundIconTexture)
				{
					Brush.SetResourceObject(StarBackgroundIconTexture);
					Image->SetBrush(Brush);
				}
			}
		}
	}
}

void URewardWidgetBase::UpdateTotalReward()
{
	if (TotalRewardBox)
	{
		TotalRewardBox->SetText(FText::FromString(FString::FromInt(CleaningReward)));
	}
}
