// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "InteractionSwitch.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionSwitch : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* Switch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AInteractionLightActor* Light;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 bIsLighting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TurnOnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TurnOffRotation;

	AInteractionSwitch();

	virtual void InterAction(APawn* Character) override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void Tick(float DeltaTime)override;

	UFUNCTION(NetMulticast, Reliable)
	void SetIntensity(float Value);
	void SetIntensity_Implementation(float Value);
};
