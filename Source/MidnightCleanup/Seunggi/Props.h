// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionPickUpObject.h"
#include "Props.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AProps : public AInteractionPickUpObject
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay()override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class AObjectOrganize> OriginalLocation;
};
