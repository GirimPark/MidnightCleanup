// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetBase.h"

#include "Kismet/GameplayStatics.h"

void UMainWidgetBase::OnRoomCreated()
{
	UGameplayStatics::OpenLevel(this, FName(*CreatedRoomLevelName), true, "listen");
}

void UMainWidgetBase::OnRoomJoined(FString IpAddress)
{
	UGameplayStatics::OpenLevel(this, FName(*IpAddress), true);
	UE_LOG(LogTemp, Warning, TEXT("%s"), TCHAR_TO_UTF8(*IpAddress));
}
