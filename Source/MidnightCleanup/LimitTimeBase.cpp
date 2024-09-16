// Fill out your copyright notice in the Description page of Project Settings.


#include "LimitTimeBase.h"
#include "Components/EditableTextBox.h"

void ULimitTimeBase::NativeConstruct()
{
	Super::NativeConstruct();

	PlayTimeBox = Cast<UEditableTextBox>(GetWidgetFromName(FName("PlayTime")));
}

void ULimitTimeBase::UpdatePlayTime(int32 InSeconds)
{
	if (PlayTimeBox)
	{
		int32 Minutes = 0;
		int32 Seconds = 0;

		Minutes = (float)InSeconds / 60.f;
		Seconds = InSeconds % 60;
		
		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PlayTimeBox->SetText(FText::FromString(TimeString));
	}
}
