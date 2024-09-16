// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/InteractionPickUpObject.h"
#include "InteractionBucket.generated.h"

UENUM(BlueprintType)
enum class EBucketState : uint8
{
	BucketState_Clean = 0 UMETA(DisplayName = "Clean"),
	BucketState_Normal = 1 UMETA(DisplayName = "Normal"),
	BucketState_Dirty = 2 UMETA(DisplayName = "Dirty")
};

UCLASS()
class MIDNIGHTCLEANUP_API AInteractionBucket : public AInteractionPickUpObject
{
	GENERATED_BODY()

public:
	AInteractionBucket();

	EBucketState BucketState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	int32 Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDurability;

	static uint8 ObjectNum;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	virtual void Tick(float DeltaTime)override;

	virtual void Destroyed()override;

	void Cleaning(APawn* Character);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsFilled:1;
};
