// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ULoginWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	//virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnLoginSucceed();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TSubclassOf<UUserWidget> MainWidgetClass;

private:

};
