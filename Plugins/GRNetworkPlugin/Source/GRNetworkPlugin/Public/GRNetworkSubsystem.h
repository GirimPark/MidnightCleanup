// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GRNetworkSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GRNETWORKPLUGIN_API UGRNetworkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromServer();

	UFUNCTION(BlueprintCallable)
	class ULogicHandler* GetLogicHandler();

public:
	UPROPERTY(BlueprintReadWrite)
	FString IpAddress = TEXT("127.0.0.1");

	UPROPERTY(BlueprintReadWrite)
	int32 Port = 5001;

private:
	FSocket* Socket;
	TSharedPtr<class GameServerSession, ESPMode::ThreadSafe> ServerSession;
	TStrongObjectPtr<class ULogicHandler> LogicHandler;
};
