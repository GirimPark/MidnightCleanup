// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API APC : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UInGameWidgetBase> InGameWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UInGameWidgetBase> InGameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class URewardWidgetBase> RewardWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class URewardWidgetBase> RewardWidget;

	void HideInventoryWidget(class APlayerCharacter* InPlayer);
	void ShowInventoryWidget(class APlayerCharacter* InPlayer);
	void HideAimWidget(class APlayerCharacter* InPlayer);
	void ShowAimWidget(class APlayerCharacter* InPlayer);
	void HideRewardWidget();
	void ShowRewardWidget(class APlayerCharacter* InPlayer);
	void HideFurnitureProgressWidget();
	void ShowFurnitureProgressWidget();
	bool IsEnableRewardWidget();
	void UpdateInventoryByUI(class APlayerCharacter* InPlayer);
	void UpdateFurnitureProgressBar(float InMaxHp, float InCurrentHp);
	void UpdateToolTip(FText InText);

	UFUNCTION(Server, Reliable)
	void C2S_OpenInitGame();
	void C2S_OpenInitGame_Implementation();
};
