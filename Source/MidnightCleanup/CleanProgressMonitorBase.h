// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CleanProgressMonitorBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UCleanProgressMonitorBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UProgressBar> FurnitureProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UProgressBar> DecalProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UProgressBar> TrashProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UProgressBar> WoodProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UEditableTextBox> FurnitureCountBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UTextBlock> DecalCountBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UTextBlock> TrashCountBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UTextBlock> WoodCountBox;


	UFUNCTION()
	void UpdateCount(int32 InFurniture, int32 InDecal, int32 InTrash, int32 InWood, int32 InitFurniture, int32 InitDecal, int32 InitTrash, int32 InitWood);

	float InitFurnitureCount = 0.f;
	float InitDecalCount = 0.f;
	float InitTrashCount = 0.f;
	float InitWoodCount = 0.f;
};
