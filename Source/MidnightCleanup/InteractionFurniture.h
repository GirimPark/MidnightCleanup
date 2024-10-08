// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/BasicInteractionObject.h"
#include "InteractionFurniture.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionFurniture : public ABasicInteractionObject
{
	GENERATED_BODY()

public:
	AInteractionFurniture();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void InterAction(APawn* Character) override;

	virtual void Destroyed()override;

	virtual void DrawOutline(bool state)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SpawnTrashNum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AInteractionWoodenPlank> Trash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bBreak : 1;

	void CreateTrash(uint8 TrashNum);

	void Axe_Hit(APawn* Character);
};