// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "ObjectOrganize.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AObjectOrganize : public ABasicInteractionObject
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual void InterAction(APawn* Character) override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	void DrawHologram(bool Draw);

	UFUNCTION()
	void ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(NetMulticast,Reliable)
	void S2A_AttachObject(AInteractionPickUpObject* Object);
	void S2A_AttachObject_Implementation(AInteractionPickUpObject* Object);
	


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AInteractionPickUpObject* TargetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;
};
