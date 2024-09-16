// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LaptopWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ULaptopWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RadialUI")
	TObjectPtr<class URequestSelectionBase> RequestSelectionBox;

};
