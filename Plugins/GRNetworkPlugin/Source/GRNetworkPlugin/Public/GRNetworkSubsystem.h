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
	void ConnectToServer(FString IpAddress, int32 Port);

	UFUNCTION(BlueprintCallable)
	void DisconnectFromServer();

	UFUNCTION(BlueprintCallable)
	class ULogicHandler* GetLogicHandler();

private:
	FSocket* Socket;
	TSharedPtr<class GameServerSession, ESPMode::ThreadSafe> ServerSession;
	TStrongObjectPtr<class ULogicHandler> LogicHandler;
};
