// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RequestSelectionBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API URequestSelectionBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UButton> StageBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UButton> StageBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UTextBlock> UserIDBox;

	UFUNCTION()
	void ClickedStageButton1();

	UFUNCTION()
	void ClickedStageButton2();
};
