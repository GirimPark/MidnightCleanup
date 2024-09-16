// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameGS.generated.h"

/**
 *
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInGameGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 TrashNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 SpawnDecalNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 OpacityDecalNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 WoodenPlankNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint8 BreakFurnitureNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitFurnitureCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitTextureDecalCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitOpacityDecalCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitTrashCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitWoodCount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayTime;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateTime)
	int32 CurrentPlayTime;

	UFUNCTION()
	void OnRep_UpdateCount();

	UFUNCTION()
	void OnRep_UpdateTime();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay()override;
};
