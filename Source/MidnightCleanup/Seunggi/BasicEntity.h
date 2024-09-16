// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicEntity.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API ABasicEntity : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UCurveFloat* Curve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PowerLevel;
};
