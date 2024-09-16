// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomDecal.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Curves/CurveFloat.h"
#include "Net/UnrealNetwork.h"
#include "Engine/EngineTypes.h"

// Sets default values
ACustomDecal::ACustomDecal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Box->SetCollisionProfileName(TEXT("Decal"));
	RootComponent = Box;

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(Box);

	SortOrder = 0;
}

// Called when the game starts or when spawned
void ACustomDecal::BeginPlay()
{
	Super::BeginPlay();	

	S2A_CreateDecalMaterial();
	S2A_SetSortOrder(SortOrder);
}

// Called every frame
void ACustomDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomDecal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACustomDecal, MinLegnth);
	DOREPLIFETIME(ACustomDecal, SortOrder);
	DOREPLIFETIME(ACustomDecal, Durability);
}

const int32 ACustomDecal::GetSortOrder()
{
	return SortOrder;
}

void ACustomDecal::S2A_SetSortOrder_Implementation(int32 InSortOrder)
{
	SortOrder = InSortOrder;
	Decal->SortOrder = SortOrder;
}

void ACustomDecal::S2A_CreateDecalMaterial_Implementation()
{
	MI = Decal->CreateDynamicMaterialInstance();
}

void ACustomDecal::S2A_SetBoxExtent_Implementation(FVector InSize)
{
	UE_LOG(LogTemp, Warning, TEXT("SetBoxExtent"));
	if (Box)
	{
		Box->SetBoxExtent(InSize);
	}
}


