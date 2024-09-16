// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientPacket.h"

class GRNETWORKPLUGIN_API AuthenticationHandler
{
public:
	AuthenticationHandler(TWeakPtr<class GameServerSession, ESPMode::ThreadSafe> Session, TStrongObjectPtr<class ULogicHandler> OwnerLogicHandler);
	~AuthenticationHandler();

	void RequestLogin(FString Username, FString Password);
	void RequestJoin(FString Username, FString Password);

/// Handler
	void HandleLoginResponse(RecvStructRef Packet);
	void HandleJoinResponse(RecvStructRef Packet);
	void HandleValidationRequest(RecvStructRef Packet);
	void HandleSessionExpiredNotification(RecvStructRef Packet);

private:
	TWeakPtr<GameServerSession, ESPMode::ThreadSafe> SessionRef;
	TWeakObjectPtr<ULogicHandler> OwnerLogicHandlerRef;
};
