// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UInGameWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void UpdateInventoryByPlayer(class APlayerCharacter* Player);
	float CalculateArrowAngle();

	int32 GetCurrentInventoryIndex();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InGame")
	TObjectPtr<class URadialUIBase> RadialUIBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InGame")
	TObjectPtr<class UUserWidget> AimBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InGame")
	TObjectPtr<class UToolTipWidgetBase> ToolTipBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InGame")
	TObjectPtr<class UFurnitureProgressBase> FurnitureProgressBox;
};
