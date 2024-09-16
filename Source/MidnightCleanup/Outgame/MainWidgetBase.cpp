// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetBase.h"

#include "Kismet/GameplayStatics.h"

void UMainWidgetBase::OnRoomCreated()
{
	UGameplayStatics::OpenLevel(this, FName(*CreatedRoomLevelName), true, "listen");
}

void UMainWidgetBase::OnRoomJoined(FName IpAddress)
{
	UGameplayStatics::OpenLevel(this, IpAddress, true);
}
