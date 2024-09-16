// Fill out your copyright notice in the Description page of Project Settings.


#include "GameRoomHandler.h"
#include "ClientPacketHandler.h"
#include "GameServerSession.h"
#include "PacketID.h"
#include "Protocol.pb.h"
#include "LogicHandler.h"


GameRoomHandler::GameRoomHandler(TWeakPtr<class GameServerSession> Session, TStrongObjectPtr<class ULogicHandler> OwnerLogicHandler) : SessionRef(Session), OwnerLogicHandlerRef(OwnerLogicHandler.Get())
{
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_CREATE_ROOM_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleCreateRoomResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_JOIN_ROOM_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleJoinRoomResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_QUIT_ROOM_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleQuitRoomResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_START_GAME_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleStartGameResponse(Packet);
		});

	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_INVITE_USER_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleInviteUserResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_INVITE_USER_NOTIFICATION, [this](RecvStructRef Packet)
		{
			HandleInviteUserNotification(Packet);
		});
}

GameRoomHandler::~GameRoomHandler()
{
}

void GameRoomHandler::RequestCreateRoom()
{
	//TSharedPtr<GameServerSession> Session = SessionRef.Pin();
	//if (!Session || !Session->IsConnected())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Session is Expired"));
	//	return;
	//}

	// Payload
	TSharedPtr<Protocol::C2S_CreateRoomRequest> CreateRoomRequest = MakeShared<Protocol::C2S_CreateRoomRequest>();

	// Header
	std::string SerializedPayload;
	CreateRoomRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_CREATE_ROOM_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = CreateRoomRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void GameRoomHandler::RequestJoinRoom(FString RoomCode)
{
	//TSharedPtr<GameServerSession> Session = SessionRef.Pin();
	//if (!Session || !Session->IsConnected())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Session is Expired"));
	//	return;
	//}

	// Payload
	TSharedPtr<Protocol::C2S_JoinRoomRequest> JoinRoomRequest = MakeShared<Protocol::C2S_JoinRoomRequest>();
	JoinRoomRequest->set_roomcode(TCHAR_TO_UTF8(*RoomCode));

	// Header
	std::string SerializedPayload;
	JoinRoomRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_JOIN_ROOM_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = JoinRoomRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void GameRoomHandler::RequestQuitRoom()
{
}

void GameRoomHandler::RequestStartGame()
{
}

void GameRoomHandler::RequestInviteUser(FString Username)
{
}

void GameRoomHandler::HandleCreateRoomResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_CreateRoomResponse CreateRoomResponse;
	CreateRoomResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (CreateRoomResponse.success().value())
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				OwnerLogicHandlerRef->OnCreateRoomSucceed.Broadcast();
			});

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
			FString::Printf(TEXT("Create Room Success, Room Code: %s"), UTF8_TO_TCHAR(CreateRoomResponse.roomcode().c_str())));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Create Room Failed")));
	}
}

void GameRoomHandler::HandleJoinRoomResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_JoinRoomResponse JoinRoomResponse;
	JoinRoomResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (JoinRoomResponse.success().value())
	{
		FString ipAddress = UTF8_TO_TCHAR(JoinRoomResponse.ipaddress().c_str());
		AsyncTask(ENamedThreads::GameThread, [this, ipAddress]()
			{
				OwnerLogicHandlerRef->OnJoinRoomSucceed.Broadcast(ipAddress);
			});

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(TEXT("Join Room Success, IP Address: %s"), UTF8_TO_TCHAR(JoinRoomResponse.ipaddress().c_str())));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Join Room Failed")));
	}
}

void GameRoomHandler::HandleQuitRoomResponse(RecvStructRef Packet)
{
}

void GameRoomHandler::HandleStartGameResponse(RecvStructRef Packet)
{
}

void GameRoomHandler::HandleInviteUserResponse(RecvStructRef Packet)
{
}

void GameRoomHandler::HandleInviteUserNotification(RecvStructRef Packet)
{
}
