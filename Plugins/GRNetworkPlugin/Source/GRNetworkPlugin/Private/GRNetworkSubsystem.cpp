// Fill out your copyright notice in the Description page of Project Settings.


#include "GRNetworkSubsystem.h"
#include "GameServerSession.h"
#include "LogicHandler.h"

#include "SocketSubsystem.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"

void UGRNetworkSubsystem::ConnectToServer(FString IpAddress, int32 Port)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);


	TArray<FString> Args;
	FString CommandLine = FCommandLine::Get();
	CommandLine.ParseIntoArrayWS(Args);

	if(!Args.IsEmpty())
	{
		FIPv4Address::Parse(Args[0], Ip);
		InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		InternetAddr->SetIp(Ip.Value);
		InternetAddr->SetPort(FCString::Atoi(*Args[1]));
	}

	if (Socket->Connect(*InternetAddr))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Session
		ServerSession = MakeShared<GameServerSession, ESPMode::ThreadSafe>(Socket);
		ServerSession->Run();

		// Logic
		if(!LogicHandler || !LogicHandler.IsValid())
			LogicHandler = TStrongObjectPtr<ULogicHandler>(NewObject<ULogicHandler>());

		if(ServerSession.IsValid() && ServerSession->IsConnected())
		{
			LogicHandler->Setup(ServerSession);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ServerSession is Expired before Setup LogicHandler"));
			return;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));

		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void UGRNetworkSubsystem::DisconnectFromServer()
{
	if (ServerSession.IsValid())
		ServerSession->Disconnect();

	Socket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	Socket = nullptr;
	LogicHandler = nullptr;
}

ULogicHandler* UGRNetworkSubsystem::GetLogicHandler()
{
	if (!LogicHandler || !LogicHandler.IsValid())
	{
		LogicHandler = TStrongObjectPtr<ULogicHandler>(NewObject<ULogicHandler>());
	}
	return LogicHandler.Get();
}
