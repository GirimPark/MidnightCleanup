// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LogicHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResponseHandlerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseHandlerDelegate_WithString, FString, StrParam);

UCLASS()
class GRNETWORKPLUGIN_API ULogicHandler : public UObject
{
	GENERATED_BODY()

public:
	ULogicHandler();
	~ULogicHandler();

	void Setup(TSharedPtr<class GameServerSession> ServerSession);


	UFUNCTION(BlueprintCallable)
	void RequestLogin(FString Username, FString Password);

	UFUNCTION(BlueprintCallable)
	void RequestJoin(FString Username, FString Password);

	UFUNCTION(BlueprintCallable)
	void RequestCreateRoom();

	UFUNCTION(BlueprintCallable)
	void RequestJoinRoom(FString RoomCode);


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnSessionExpired;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnLoginSucceed;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnCreateRoomSucceed;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithString OnJoinRoomSucceed;

private:
	TWeakPtr<class GameServerSession, ESPMode::ThreadSafe> SessionRef;
	TSharedPtr<class AuthenticationHandler, ESPMode::ThreadSafe> AuthenticationHandler;
	TSharedPtr<class GameRoomHandler, ESPMode::ThreadSafe> GameRoomHandler;
};
