// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"
#include "BasicGhost.h"
#include "EngineUtils.h"
#include "Components\SplineComponent.h"
#include "ObjectSpawnPoint.h"
#include "../InteractionBox.h"
#include "Components\BoxComponent.h"
#include "../TextureDecal.h"
#include "../OpacityDecal.h"
#include "../InteractionTrash.h"
#include "../InteractionFurniture.h"
#include "InteractionWoodenPlank.h"
#include "../CleanProgressMonitor.h"
#include "../LimitTimeMonitor.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../InteractionFurniture.h"
#include "InteractionWoodenPlank.h"
#include "../OpacityDecal.h"
#include "../TextureDecal.h"
#include "InGameGS.h"

AInGameGM::AInGameGM()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInGameGM::BeginPlay()
{
	Super::BeginPlay();

	InitCountObject();

	GetWorldTimerManager().SetTimer(TimerHandle,this,&AInGameGM::DecreaseTime, 1.0f, true, 1.0f);

	UWorld* World = GetWorld();

	for (TActorIterator<APointBase> It(World); It; ++It)
	{
		ASpawnPoint* sp = Cast<ASpawnPoint>(*It);
		if (sp)
		{
			for (FEntityPowerLevel& Entity : Entities)
			{
				if (Entity.EntityType == EEntityType::Ghost&&sp->SpawnType==ESpawnType::Ghost)
				{
					Entity.SpawnPoint.Add(sp);
				}
				else if (Entity.EntityType == EEntityType::Event && sp->SpawnType == ESpawnType::Event)
				{
					Entity.SpawnPoint.Add(sp);
				}
			}
		}
		else
		{
			RunAwayPoints.Add(*It);
		}
	}

	if (CleanProgressMonitorClass)
	{
		FVector Location = FVector(-2439.049223, -755.325583, 479.05854);
		FRotator Rotation = FRotator(0, 180, 0);
		FVector Scale = FVector(0.15, 0.122828, 0.107198);
		FTransform Transform;
		Transform.SetTranslation(Location);
		Transform.SetRotation(Rotation.Quaternion());
		Transform.SetScale3D(Scale);
		C2S_SpawnCleanProgressMonitor(CleanProgressMonitorClass, Transform);
	}

	if (LimitTimesMonitorClass)
	{
		FVector Location = FVector(-2580.0f, -940.f, 480.f);
		FRotator Rotation = FRotator(0.f, 90.f, 0.f);
		FVector Scale = FVector(0.1f, 0.1f, 0.1f);
		FTransform Transform;
		Transform.SetTranslation(Location);
		Transform.SetRotation(Rotation.Quaternion());
		Transform.SetScale3D(Scale);
		C2S_SpawnLimitTimeMonitor(LimitTimesMonitorClass, Transform);
	}
}

void AInGameGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if (SpawnAI.Num() < MaxGhostNum)
	//{
	//	if (TrashGhost)
	//	{
	//		TrashSpendTime += DeltaTime;
	//
	//		if (TrashSpendTime >= TrashGhostSpawnTime && TrashGhostSpawnPoints.Num() > 0)
	//		{
	//			int index = FMath::RandRange(0, TrashGhostSpawnPoints.Num() - 1);
	//			SpawnAICharacter(TrashGhost, TrashGhostSpawnPoints[index]);
	//			TrashSpendTime = 0;
	//		}
	//	}
	//	if (LightGhost)
	//	{
	//		LightSpendTime += DeltaTime;
	//
	//		if (LightSpendTime >= LightGhostSpawnTime && LightGhostSpawnPoints.Num() > 0)
	//		{
	//			int index = FMath::RandRange(0, LightGhostSpawnPoints.Num() - 1);
	//			SpawnAICharacter(LightGhost, LightGhostSpawnPoints[index]);
	//			LightSpendTime = 0;
	//		}
	//	}
	//	if (DoorGhost)
	//	{
	//		DoorSpendTime += DeltaTime;
	//
	//		if (DoorSpendTime >= DoorGhostSpawnTime && DoorGhostSpawnPoints.Num() > 0)
	//		{
	//			int index = FMath::RandRange(0, DoorGhostSpawnPoints.Num() - 1);
	//			SpawnAICharacter(DoorGhost, DoorGhostSpawnPoints[index]);
	//			DoorSpendTime = 0;
	//		}
	//	}
	//}

}

void AInGameGM::SpawnAICharacter_Implementation(TSubclassOf<ABasicGhost> Ghost, AGhostSpawnPoint* SpawnPoint)
{
	if (Ghost)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Transform = FTransform(SpawnPoint->GetActorLocation());
		ABasicGhost* SpawnedGhost = GetWorld()->SpawnActor<ABasicGhost>(Ghost, Transform, SpawnParam);
		SpawnedGhost->PatrolPoints = SpawnPoint->PatrolPoints;
		SpawnAI.Add(SpawnedGhost);
	}
}

void AInGameGM::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bUseSeamlessTravel = true;
}

void AInGameGM::ChangeLevel(FString InLevelName)
{
	GetWorld()->ServerTravel(InLevelName);
}

void AInGameGM::CanSpawnEntity(FEntityPowerLevel Object,int32 InTime)
{
	if (MaxPowerLevel - CurrentPowerLevel < Object.PowerLevel)
	{
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Try Spawn Entity"));
		UE_LOG(LogTemp, Warning, TEXT("Percent Per Time : %f"), Object.CurveFloat->GetFloatValue(InTime));
		int RandomNum = FMath::RandRange(0, 100);
		if (RandomNum <= Object.CurveFloat->GetFloatValue(InTime))
		{
			
			if (Object.Entity->IsChildOf<ABasicGhost>())
			{
				if (Object.SpawnPoint.Num() > 0)
				{
					int index = FMath::RandRange(0, Object.SpawnPoint.Num() - 1);
					AGhostSpawnPoint* SpawnPoint = Cast<AGhostSpawnPoint>(Object.SpawnPoint[index]);
					if(SpawnPoint)
					{
						UE_LOG(LogTemp, Warning, TEXT("Spawn Entity"));
						SpawnEntity(Object.Entity, SpawnPoint);
					}
					CurrentPowerLevel += Object.PowerLevel;
				}
			}
		}
	}

}

void AInGameGM::SpawnBox_Implementation(TSubclassOf<class AInteractionBox> Box, AObjectSpawnPoint* SpawnPoint)
{
	if (Box)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Transform = FTransform(SpawnPoint->GetActorLocation());
		AInteractionBox* SpawnedBox = GetWorld()->SpawnActor<AInteractionBox>(Box, Transform, SpawnParam);

		//SpawnedBox->Box->SetSimulatePhysics(true);
	}
}

void AInGameGM::C2S_SpawnCleanProgressMonitor_Implementation(TSubclassOf<class ACleanProgressMonitor> Monitor, FTransform Transform)
{
	if (Monitor)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CleanProgressMonitor = GetWorld()->SpawnActor<ACleanProgressMonitor>(Monitor, Transform, SpawnParam);
		UE_LOG(LogTemp, Warning, TEXT("GM : SpawnWidget"));
		if (CleanProgressMonitor)
		{
			CleanProgressMonitor->Widget->SetDrawSize(FVector2D(1920.f, 1080.f));
			CleanProgressMonitor->Widget->SetWidgetSpace(EWidgetSpace::World);
		}
	}
}

void AInGameGM::C2S_SpawnLimitTimeMonitor_Implementation(TSubclassOf<class ALimitTimeMonitor> Monitor, FTransform Transform)
{
	if (Monitor)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		LimitTimeMonitor = GetWorld()->SpawnActor<ALimitTimeMonitor>(Monitor, Transform, SpawnParam);
		UE_LOG(LogTemp, Warning, TEXT("GM : SpawnWidget"));
		if (LimitTimeMonitor)
		{
			LimitTimeMonitor->Widget->SetDrawSize(FVector2D(1920.f, 1080.f));
			LimitTimeMonitor->Widget->SetWidgetSpace(EWidgetSpace::World);
		}
	}
}

void AInGameGM::OpenInitGame()
{
	GetWorld()->ServerTravel(TEXT("L_InitGame"));
}

void AInGameGM::InitCountObject()
{
	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		TArray<AActor*> Objects;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionFurniture::StaticClass(), Objects);
		GS->InitFurnitureCount = Objects.Num();
		GS->InitWoodCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionTrash::StaticClass(), Objects);
		GS->InitTrashCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATextureDecal::StaticClass(), Objects);
		GS->InitTextureDecalCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOpacityDecal::StaticClass(), Objects);
		GS->InitOpacityDecalCount = Objects.Num();

		CountObject();
		GS->OnRep_UpdateCount();


		UE_LOG(LogTemp, Warning, TEXT("InitCountObject : %f, %f, %f, %f, %f"), GS->InitFurnitureCount, GS->InitWoodCount, GS->InitTrashCount, GS->InitTextureDecalCount, GS->InitOpacityDecalCount);
	}
}

void AInGameGM::CountObject()
{
	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		TArray<AActor*> Objects;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionFurniture::StaticClass(), Objects);
		GS->BreakFurnitureNum = Objects.Num();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionWoodenPlank::StaticClass(), Objects);
		int32 num = 0;
		for (AActor* actor : Objects)
		{
			AInteractionWoodenPlank* WoodenPlank = Cast<AInteractionWoodenPlank>(actor);
			if (WoodenPlank&&!WoodenPlank->bIsCleaning)
			{
				++num;
			}
		}
		GS->WoodenPlankNum = num;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionTrash::StaticClass(), Objects);
		GS->TrashNum = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATextureDecal::StaticClass(), Objects);
		GS->SpawnDecalNum = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOpacityDecal::StaticClass(), Objects);
		GS->OpacityDecalNum = Objects.Num();

		GS->OnRep_UpdateCount();

		UE_LOG(LogTemp, Warning, TEXT("CountObject : %d, %d, %d, %d, %d"), GS->BreakFurnitureNum, GS->WoodenPlankNum, GS->TrashNum, GS->SpawnDecalNum, GS->OpacityDecalNum);
	}
}


void AInGameGM::SpawnEntity_Implementation(TSubclassOf<AActor> Entity, AGhostSpawnPoint* SpawnPoint)
{
	if (Entity->IsChildOf<ABasicGhost>())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Transform = FTransform(SpawnPoint->GetActorLocation());
		ABasicGhost* SpawnedGhost = GetWorld()->SpawnActor<ABasicGhost>(Entity, Transform, SpawnParam);
		SpawnedGhost->PatrolPoints = SpawnPoint->PatrolPoints;
	}
}

void AInGameGM::DecreaseTime()
{

	AInGameGS* GS = GetGameState<AInGameGS>();
	if(GS)
	{
		GS->CurrentPlayTime--;
		GS->OnRep_UpdateTime();

		UE_LOG(LogTemp, Warning, TEXT("DecreaseTime"));
		int32 Time = GS->PlayTime - GS->CurrentPlayTime;
		if (EventTimeIndex<EventTime.Num()&& Time == EventTime[EventTimeIndex])
		{
			for (FEntityPowerLevel Entity : Entities)
			{
				CanSpawnEntity(Entity, Time);
			}
			if(CurrentPowerLevel < 5)
			{
				for(int i = 0;i<100;i++)
				{
					for (FEntityPowerLevel Entity : Entities)
					{
						if (CurrentPowerLevel > 5)
							break;
						CanSpawnEntity(Entity, Time);
					}
				}
			}
			EventTimeIndex++;
		}
	}


}

