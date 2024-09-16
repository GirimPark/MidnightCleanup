// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicHandler.h"
#include "AuthenticationHandler.h"
#include "GameRoomHandler.h"
#include "GameServerSession.h"

ULogicHandler::ULogicHandler()
{
}

ULogicHandler::~ULogicHandler()
{
	OnSessionExpired.Clear();
	OnLoginSucceed.Clear();
	OnCreateRoomSucceed.Clear();
}

void ULogicHandler::Setup(TSharedPtr<GameServerSession> ServerSession)
{
	SessionRef = ServerSession;
	AuthenticationHandler = MakeShared<class AuthenticationHandler, ESPMode::ThreadSafe>(SessionRef, TStrongObjectPtr<ULogicHandler>(this));
	GameRoomHandler = MakeShared<class GameRoomHandler, ESPMode::ThreadSafe>(SessionRef, TStrongObjectPtr<ULogicHandler>(this));
}

void ULogicHandler::RequestLogin(FString Username, FString Password)
{
	AuthenticationHandler->RequestLogin(Username, Password);
}

void ULogicHandler::RequestJoin(FString Username, FString Password)
{
	AuthenticationHandler->RequestJoin(Username, Password);
}

void ULogicHandler::RequestCreateRoom()
{
	GameRoomHandler->RequestCreateRoom();
}

void ULogicHandler::RequestJoinRoom(FString RoomCode)
{
	GameRoomHandler->RequestJoinRoom(RoomCode);
}
