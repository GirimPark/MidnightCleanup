// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPickUpObject.h"
#include "Components\BoxComponent.h"
#include "BasicGhost.h"
#include "../PlayerCharacter.h"
#include "../BasicTool.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet\GameplayStatics.h"
#include "Sound\SoundCue.h"
#include "Components/AudioComponent.h"

AInteractionPickUpObject::AInteractionPickUpObject()
{
	SetReplicateMovement(true);
	Box->SetSimulatePhysics(true);
	Box->SetUseCCD(true);
	ObjectType = EObjectType::ObjectType_PickUp;
	Box->OnComponentHit.AddDynamic(this, &AInteractionPickUpObject::ProcessBeginOverlap);
	//Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractionPickUpObject::ProcessBeginOverlap);
}


void AInteractionPickUpObject::BeginPlay()
{
	Super::BeginPlay();
	
	AudioComponent = NewObject<UAudioComponent>(this);
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetSound(DropSound);  // DropSound는 사운드 큐로 설정

	AudioComponent->RegisterComponent();
}

void AInteractionPickUpObject::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		if (!Player->bIsHeld)
		{
			SetObjectOwner(Character);
			PickUp(Character);
		}
	}
	else
	{
		PickUp(Character);
	}
	
}

void AInteractionPickUpObject::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AInteractionPickUpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractionPickUpObject::PickUp_Implementation(APawn* Character)
{
	//Box->SetSimulatePhysics(false);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
	ABasicGhost* GhostCharacter = Cast<ABasicGhost>(Character);
	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);

	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,     
		true                           
	);
	
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pick up Something : Cur(%d), Prev(%d)"), PlayerCharacter->CurrentInventoryIndex, PlayerCharacter->PreviousInventoryIndex);
		PlayerCharacter->AddInventoryCurrentIndex(this);

		//if (PickUpObjectType == EPickUpObjectType::PickUpObjectType_Box)
		//{
		//	PlayerCharacter->bIsHeldBox = true;
		//
		//	//PickUpTrash(Character);
		//	PickUpObject(PlayerCharacter);
		//}
		UE_LOG(LogTemp, Warning, TEXT("Pick up Something"))
		
		//AttachToComponent(PlayerCharacter->GetMesh(), AttachmentRules, FNam("Box"));
		Box->SetSimulatePhysics(true);
		DrawOutline(false);
		PickUpObject(PlayerCharacter);
		
	}

	else if (GhostCharacter)
	{
		PickUpObject(GhostCharacter);
		//Box->SetSimulatePhysics(false);
	}
	
}

void AInteractionPickUpObject::Drop_Implementation(APawn* Character)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
	FDetachmentTransformRules DetachmentRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		false
	);

	if (PlayerCharacter)
	{
		PlayerCharacter->RemoveInventoryIndex(PlayerCharacter->CurrentInventoryIndex);
		//DropObject(PlayerCharacter);

		if (PickUpObjectType == EPickUpObjectType::PickUpObjectType_Box)
		{
			PlayerCharacter->bIsHeldBox = false;
			//DropTrash(Character);
		}
	}

	//DetachFromActor(DetachmentRules);
	DropProcess(Character);
	//APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	//ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	//if (Player)
	//{
	//	Player->PhysicsHandle->ReleaseComponent();
	//
	//	if (Player->PhysicsHandle->GetGrabbedComponent())
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("GrabbedComponent : %s"), *Player->PhysicsHandle->GetGrabbedComponent()->GetName());
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("GrabbedComponent is Empty"));
	//	}
	//	
	//}
	//else if (Ghost)
	//{
	//	Ghost->PhysicsHandle->ReleaseComponent();
	//}
}

void AInteractionPickUpObject::DropPrevious_Implementation(APawn* Character)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
	FDetachmentTransformRules DetachmentRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		false
	);

	if (PlayerCharacter)
	{
		PlayerCharacter->RemoveInventoryIndex(PlayerCharacter->PreviousInventoryIndex);

		if (PickUpObjectType == EPickUpObjectType::PickUpObjectType_Box)
		{
			PlayerCharacter->bIsHeldBox = false;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Drop_Implementation"));
	DropProcess(Character);
}

void AInteractionPickUpObject::PickUpObject(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	if(Player)
	{
		FVector location = GetActorLocation();//StaticMesh->GetSocketLocation(FName("Socket"));
		Player->PhysicsHandle->GrabComponentAtLocationWithRotation(Box, FName("Box"), location, GetActorRotation());
		UE_LOG(LogTemp, Warning, TEXT("Pick up Object"))
		
	}
	else if (Ghost)
	{
		Ghost->PhysicsHandle->GrabComponentAtLocationWithRotation(Box, NAME_None, GetActorLocation(), GetActorRotation());
		UE_LOG(LogTemp, Warning, TEXT("Ghost Pick up Object"))
	}
}

void AInteractionPickUpObject::PickUpTool(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	if (Player)
	{
		Player->PhysicsHandle->GrabComponentAtLocationWithRotation(Box, NAME_None, GetActorLocation(), GetActorRotation());
	}
	else if (Ghost)
	{
		Ghost->PhysicsHandle->GrabComponentAtLocationWithRotation(Box, FName("Tool"), GetActorLocation(), GetActorRotation());
	}
}

void AInteractionPickUpObject::SetObjectOwner(APawn* Character)
{
	if (ObjectOwner && ObjectOwner != Character)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
		if (Player)
		{
			Player->DropAbsolutely();
			UE_LOG(LogTemp, Warning, TEXT("Steal OtherPlayer Object"));
		}
	}
	 
	S2A_SetObjectOwner(Character);
}

void AInteractionPickUpObject::S2A_SetObjectOwner_Implementation(APawn* Character)
{
	ObjectOwner = Character;
	bIsCleaning = false;
}

void AInteractionPickUpObject::S2A_SetDuplicateActor_Implementation(AInteractionPickUpObject* Actor)
{
	DuplicateActor = Actor;
}

void AInteractionPickUpObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionPickUpObject, ObjectOwner);
	DOREPLIFETIME(AInteractionPickUpObject, DuplicateActor);
}

void AInteractionPickUpObject::ProcessBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit)
{
	if (ObjectOwner)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("impulse size : %f"), NormalImpulse.Size());
	if (NormalImpulse.Size() > 100)
	{
		if (DropSound&& AudioComponent)
		{
			if (!AudioComponent->IsPlaying())
			{
				AudioComponent->Play();
				UE_LOG(LogTemp, Warning, TEXT("Playing sound"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Sound is already playing"));
			}
		}
	}
}

void AInteractionPickUpObject::DropObject(APawn* Character)
{
	S2A_SetObjectOwner(nullptr);
	Drop(Character);
}

void AInteractionPickUpObject::DropPreviousObject(APawn* Character)
{
	S2A_SetObjectOwner(nullptr);
	DropPrevious(Character);
}

void AInteractionPickUpObject::DropProcess(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	if (Player)
	{
		Player->PhysicsHandle->ReleaseComponent();
		Player->OwnedActor = nullptr;
	}
	else if (Ghost)
	{
		Ghost->PhysicsHandle->ReleaseComponent();
	}
}
