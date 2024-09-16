// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientPacket.h"

class GRNETWORKPLUGIN_API GameRoomHandler
{
public:
	GameRoomHandler(TWeakPtr<class GameServerSession> Session, TStrongObjectPtr<class ULogicHandler> OwnerLogicHandler);
	~GameRoomHandler();

	void RequestCreateRoom();
	void RequestJoinRoom(FString RoomCode);
	void RequestQuitRoom();
	void RequestStartGame();

	void RequestInviteUser(FString Username);

	/// Handler
	void HandleCreateRoomResponse(RecvStructRef Packet);
	void HandleJoinRoomResponse(RecvStructRef Packet);
	void HandleQuitRoomResponse(RecvStructRef Packet);
	void HandleStartGameResponse(RecvStructRef Packet);

	void HandleInviteUserResponse(RecvStructRef Packet);
	void HandleInviteUserNotification(RecvStructRef Packet);

private:
	TWeakPtr<GameServerSession, ESPMode::ThreadSafe> SessionRef;
	TWeakObjectPtr<ULogicHandler> OwnerLogicHandlerRef;
};
