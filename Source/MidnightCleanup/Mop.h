// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTool.h"
#include "Mop.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API AMop : public ABasicTool
{
	GENERATED_BODY()

public:
	AMop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 EraseCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 CleanDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 DirtyDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FVector DecalSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TSubclassOf<class ATextureDecal> DecalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UMaterialInstance* DecalMaterial;

	UPROPERTY(VisibleAnywhere, Replicated)
	UMaterialInstance* HeadMI = nullptr;

	UPROPERTY(VisibleAnywhere, Replicated)
	UMaterialInstance* StickMI = nullptr;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnTool()override;

	virtual void DrawOutline(bool Draw) override;

	UFUNCTION(Server, Reliable)
	void C2S_SetMaxDurability();
	void C2S_SetMaxDurability_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_SetMopDurability(int32 InDurability);
	void C2S_SetMopDurability_Implementation(int32 InDurability);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetMopActivate(bool State);
	void S2A_SetMopActivate_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_DecreaseDurability(class ATextureDecal* InDecal);
	void C2S_DecreaseDurability_Implementation(class ATextureDecal* InDecal);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetMopDurability(int32 InDurability);
	void S2A_SetMopDurability_Implementation(int32 InDurability);

	void CreateDecal(FVector ImpactPoint, FVector Normal);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_CreateMopMaterial();
	void S2A_CreateMopMaterial_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetMopMaterial(int32 InDurability);
	void S2A_SetMopMaterial_Implementation(int32 InDurability);
};
