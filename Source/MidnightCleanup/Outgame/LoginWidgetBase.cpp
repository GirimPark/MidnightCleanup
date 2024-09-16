// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidgetBase.h"

void ULoginWidgetBase::OnLoginSucceed()
{

	if(UUserWidget* MainWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass))
	{
		MainWidget->AddToViewport();
	}
}
