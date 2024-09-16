// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTool.h"
#include "Sponge.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API ASponge : public ABasicTool
{
	GENERATED_BODY()
	
public:
	ASponge();

	UPROPERTY(VisibleAnywhere, Replicated)
	class AOpacityDecal* OverlappedDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float OpacityRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 CleanDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 DirtyDurability;

	UPROPERTY(VisibleAnywhere, Replicated)
	UMaterialInstance* MI = nullptr;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable)
	void C2S_SetMaxDurability();
	void C2S_SetMaxDurability_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_DecreaseDurability();
	void C2S_DecreaseDurability_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetSpongeDurability(int32 InDurability);
	void S2A_SetSpongeDurability_Implementation(int32 InDurability);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetSpongeActivate(bool State);
	void S2A_SetSpongeActivate_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_DestoryOverlappedDecal();
	void S2A_DestoryOverlappedDecal_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetOverlappedDecal(class AOpacityDecal* Decal);
	void S2A_SetOverlappedDecal_Implementation(class AOpacityDecal* Decal);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_CreateSpongeMaterial();
	void S2A_CreateSpongeMaterial_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetSpongeMaterial(int32 InDurability);
	void S2A_SetSpongeMaterial_Implementation(int32 InDurability);
};
