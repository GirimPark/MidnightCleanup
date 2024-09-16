// Fill out your copyright notice in the Description page of Project Settings.


#include "LaptopWidgetBase.h"
#include "RequestSelectionBase.h"

void ULaptopWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	RequestSelectionBox = Cast<URequestSelectionBase>(GetWidgetFromName(TEXT("RequestSelection")));
}
