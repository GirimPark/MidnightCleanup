// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InitGameGM.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInitGameGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void OpenStage1();
	void OpenStage2();
	void OpenInGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StageName1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StageName2;
	
};
