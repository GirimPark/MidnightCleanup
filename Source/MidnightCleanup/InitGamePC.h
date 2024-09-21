// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InitGamePC.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInitGamePC : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class ULaptopWidgetBase> LaptopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class ULaptopWidgetBase> LaptopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class ULaptopWidgetBase> ToolTipWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class ULaptopWidgetBase> ToolTipWidget;

	virtual void BeginPlay() override;

	void ShowLaptopWidget();

	UFUNCTION(Server, Reliable)
	void C2S_OpenInGame();
	void C2S_OpenInGame_Implementation();
};
