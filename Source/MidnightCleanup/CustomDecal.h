// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomDecal.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API ACustomDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomDecal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> Box;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDecalComponent> Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float MinLegnth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	int32 Durability;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* MI;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 SortOrder;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const int32 GetSortOrder();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetSortOrder(int32 InSortOrder);
	void S2A_SetSortOrder_Implementation(int32 InSortOrder);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_CreateDecalMaterial();
	void S2A_CreateDecalMaterial_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetBoxExtent(FVector InSize);
	void S2A_SetBoxExtent_Implementation(FVector InSize);
};
