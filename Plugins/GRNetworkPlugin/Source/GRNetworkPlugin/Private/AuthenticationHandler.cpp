// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthenticationHandler.h"
#include "ClientPacketHandler.h"
#include "GameServerSession.h"
#include "PacketID.h"
#include "Protocol.pb.h"
#include "LogicHandler.h"


AuthenticationHandler::AuthenticationHandler(TWeakPtr<GameServerSession, ESPMode::ThreadSafe> Session, TStrongObjectPtr<ULogicHandler> OwnerLogicHandler) : SessionRef(Session), OwnerLogicHandlerRef(OwnerLogicHandler.Get())
{
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_LOGIN_RESPONSE , [this](RecvStructRef Packet)
	{
		HandleLoginResponse(Packet);
	});

	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_JOIN_RESPONSE, [this](RecvStructRef Packet)
	{
		HandleJoinResponse(Packet);
	});

	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_VALIDATION_REQUEST, [this](RecvStructRef Packet)
	{
		HandleValidationRequest(Packet);
	});

	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_SESSION_EXPIRED_NOTIFICATION, [this](RecvStructRef Packet)
	{
		HandleSessionExpiredNotification(Packet);
	});
}

AuthenticationHandler::~AuthenticationHandler()
{
}

void AuthenticationHandler::RequestLogin(FString Username, FString Password)
{
	//TSharedPtr<GameServerSession> Session = SessionRef.Pin();
	//if (!Session || !Session->IsConnected())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Session is Expired"));
	//	return;
	//}

	// Payload
	TSharedPtr<Protocol::C2S_LoginRequest> LoginRequest = MakeShared<Protocol::C2S_LoginRequest>();
	LoginRequest->set_username(TCHAR_TO_UTF8(*Username));
	LoginRequest->set_password(TCHAR_TO_UTF8(*Password));

	// Header
	std::string SerializedPayload;
	LoginRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header = 
	MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_LOGIN_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = LoginRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::RequestJoin(FString Username, FString Password)
{
	//TSharedPtr<GameServerSession> Session = SessionRef.Pin();
	//if (!Session || !Session->IsConnected())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Session is Expired"));
	//	return;
	//}

	// Payload
	TSharedPtr<Protocol::C2S_JoinRequest> JoinRequest = MakeShared<Protocol::C2S_JoinRequest>();
	JoinRequest->set_username(TCHAR_TO_UTF8(*Username));
	JoinRequest->set_password(TCHAR_TO_UTF8(*Password));

	// Header
	std::string SerializedPayload;
	JoinRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_JOIN_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = JoinRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::HandleLoginResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_LoginResponse LoginResponse;
	LoginResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (LoginResponse.success().value())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Success")));

		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				OwnerLogicHandlerRef->OnLoginSucceed.Broadcast();
			});
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Failed")));
	}
}

void AuthenticationHandler::HandleJoinResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_JoinResponse JoinResponse;
	JoinResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (JoinResponse.success().value())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Join Success")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Join Failed")));
	}
}

void AuthenticationHandler::HandleValidationRequest(RecvStructRef Packet)
{
	// 역직렬화 필요 없이, 바로 Validation Response 보내기
	//TSharedPtr<GameServerSession> Session = SessionRef.Pin();
	//if(!Session || !Session->IsConnected())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Session is Expired"));
	//	return;
	//}

	// Payload
	TSharedPtr<Protocol::C2S_ValidationResponse> ValidationResponse = MakeShared<Protocol::C2S_ValidationResponse>();

	// Header
	std::string SerializedPayload;
	ValidationResponse->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_VALIDATION_RESPONSE));

	// SendPacket
	SendStructRef SendPacket = MakeShared<SendStruct>();
	SendPacket->Header = Header;
	SendPacket->Payload = ValidationResponse;

	SessionRef.Pin()->InsertSendTask(SendPacket);
}

void AuthenticationHandler::HandleSessionExpiredNotification(RecvStructRef Packet)
{
	UE_LOG(LogTemp, Error, TEXT("Session Expired Notification Packet is Handled"));

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnSessionExpired.Broadcast();
		});
	

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Session Expired")));
}
