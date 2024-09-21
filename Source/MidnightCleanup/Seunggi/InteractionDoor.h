// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "InteractionDoor.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionDoor : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:
	AInteractionDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ClosedPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OpenedPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsOpened : 1;

	virtual void InterAction(APawn* Player) override;
	virtual void DrawOutline(bool Draw) override;
	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void DoorMove();
	void DoorMove_Implementation();
};
