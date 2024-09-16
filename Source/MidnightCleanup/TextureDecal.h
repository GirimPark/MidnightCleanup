// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDecal.h"
#include "TextureDecal.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ATextureDecal : public ACustomDecal
{
	GENERATED_BODY()

public:
	ATextureDecal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Replicated)
	FVector BaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	class UTexture2D* AlphaTexture_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	class UTexture2D* AlphaTexture_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	class UTexture2D* AlphaTexture_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Replicated)
	float OpacityLevel_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Replicated)
	float OpacityLevel_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Replicated)
	float Contrast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Replicated)
	float Roughness;

	virtual void BeginPlay() override;
	virtual void Destroyed()override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetAlphaTexture(class UTexture* Texture);
	void S2A_SetAlphaTexture_Implementation(class UTexture* Texture);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetSize(FVector InSize);
	void S2A_SetSize_Implementation(FVector InSize);

	UFUNCTION(Server, Reliable)
	void C2S_SetAlphaTextureAndOpacity();
	void C2S_SetAlphaTextureAndOpacity_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetOpacityLevel(float InOpacity);
	void S2A_SetOpacityLevel_Implementation(float InOpacity);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_Destroy();
	void S2A_Destroy_Implementation();
};
