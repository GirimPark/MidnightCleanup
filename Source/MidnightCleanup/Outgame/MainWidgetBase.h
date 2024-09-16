// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void OnRoomCreated();

	UFUNCTION(BlueprintCallable)
	void OnRoomJoined(FName IpAddress);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FString CreatedRoomLevelName;
};
