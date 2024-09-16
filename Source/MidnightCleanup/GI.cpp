// Fill out your copyright notice in the Description page of Project Settings.


#include "GI.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"

void UGI::Init()
{
	Super::Init();

#if UE_BUILD_SHIPPING
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UGI::InitWindowSetting, 0.1f, false);
#endif
}

void UGI::ShowSessionExpiredWidget()
{
	if(!WBP_SessionExpiredAlertClass)
		UE_LOG(LogTemp, Error, TEXT("WBP_SessionExpiredAlertClass is nullptr"))
	
	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WBP_SessionExpiredAlertClass);
	if(WidgetInstance)
	{
		WidgetInstance->AddToViewport();
	}
}

FString UGI::GetUserID()
{
	return UserID;
}

void UGI::SetUserID(FString NewUserID)
{
	UE_LOG(LogTemp, Warning, TEXT("UGI::SetUserID()"));
	UserID = NewUserID;
}

void UGI::InitWindowSetting()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		Settings->SetScreenResolution(FIntPoint(1920, 1080));
		Settings->ApplySettings(false);
	}
}
