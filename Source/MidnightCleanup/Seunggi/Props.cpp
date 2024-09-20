// Fill out your copyright notice in the Description page of Project Settings.


#include "Props.h"
#include "ObjectOrganize.h"

void AProps::BeginPlay()
{
	Super::BeginPlay();
	if (bIsCleaning)
	{
		if(HasAuthority())
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FTransform Transform = FTransform(GetActorLocation());
			AObjectOrganize* SpawnedOriginalLocation = GetWorld()->SpawnActor<AObjectOrganize>(OriginalLocation, Transform, SpawnParam);

			SpawnedOriginalLocation->TargetObject = this;
			SpawnedOriginalLocation->Initialize();
		}
	}
}
