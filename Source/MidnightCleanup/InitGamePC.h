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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class ULaptopWidgetBase> LaptopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class ULaptopWidgetBase> LaptopWidget;

	virtual void BeginPlay() override;

	void ChangeClientLevel();
	
};
