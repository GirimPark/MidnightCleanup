// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectOrganize.h"
#include "../PlayerCharacter.h"
#include "Components\BoxComponent.h"
#include "InteractionPickUpObject.h"

void AObjectOrganize::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AObjectOrganize::ProcessBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AObjectOrganize::ProcessEndOverlap);

	if(TargetObject)
	{
		StaticMesh->SetStaticMesh(TargetObject->StaticMesh->GetStaticMesh());
		
		if (MaterialInstance)
		{
			StaticMesh->SetMaterial(0, MaterialInstance);
			StaticMesh->SetMaterial(1, MaterialInstance);
		}
		StaticMesh->SetVisibility(false);
	}
}

void AObjectOrganize::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*> Array;
	Box->GetOverlappingActors(Array);
	for (AActor* Actor : Array)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if (Player && Player->IsLocallyControlled())
		{
			if (StaticMesh->IsVisible())
			{
				if (Player->OwnedActor == nullptr)
				{
					StaticMesh->SetVisibility(false);
				}
			}
			else
			{				
				if (Player->OwnedActor == TargetObject)
				{
					StaticMesh->SetVisibility(true);
				}
			}
		}
	}
}

void AObjectOrganize::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		if (Player->OwnedActor == nullptr)
		{
			return;
		}
		else
		{
			if (TargetObject == Player->OwnedActor)
			{
				Player->DropObject();
				
				S2A_AttachObject(TargetObject);
			}
		}
	}
}

void AObjectOrganize::DrawOutline(bool Draw)
{
	
}

void AObjectOrganize::DrawHologram(bool Draw)
{
	StaticMesh->SetVisibility(Draw);
}

void AObjectOrganize::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player&&Player->IsLocallyControlled()&&TargetObject == Player->OwnedActor)
	{
		DrawHologram(true);
	}
}

void AObjectOrganize::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && Player->IsLocallyControlled() && StaticMesh->IsVisible())
	{
		DrawHologram(false);
	}
}

void AObjectOrganize::S2A_AttachObject_Implementation(AInteractionPickUpObject* Object)
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);

	Object->AttachToComponent(StaticMesh, AttachmentRules);
	Object->DetachFromActor(DetachmentRules);
}

