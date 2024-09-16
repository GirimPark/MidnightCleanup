// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RewardWidgetBase.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API URewardWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reward")
	TObjectPtr<class UTextBlock> UserIDBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reward")
	TObjectPtr<class UButton> ReturnBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reward")
	TObjectPtr<class UTextBlock> CleaningRewardBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reward")
	TObjectPtr<class UTextBlock> TotalRewardBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reward")
	TObjectPtr<class UHorizontalBox> StarLayoutBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	TObjectPtr<class UTexture2D> StarIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	TObjectPtr<class UTexture2D> StarBackgroundIconTexture;

	UFUNCTION()
	void ClickedReturnButton();

	void UpdateCleaningReward();

	void UpdateStarIcon();
	void UpdateTotalReward();

	int32 CleaningReward = 0;
	int32 StarNum = 0;
};
