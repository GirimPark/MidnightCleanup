// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTool.h"
#include "Flashlight.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AFlashlight : public ABasicTool
{
	GENERATED_BODY()

public:
	AFlashlight();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULocalLightComponent> LightClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpotLightComponent* SpotLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float Intensity;

public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetFlashLightOn(bool State);
	void S2A_SetFlashLightOn_Implementation(bool State);

	void OnFlashlight(bool State);
};
