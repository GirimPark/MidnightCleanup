// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UGI : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	FString GetUserID();
	
	UFUNCTION(BlueprintCallable)
	void SetUserID(FString NewUserID);

	UFUNCTION(BlueprintCallable)
	void ShowSessionExpiredWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bDrawDebug : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> WBP_SessionExpiredAlertClass;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UserID;

	void InitWindowSetting();
};
