// Fill out your copyright notice in the Description page of Project Settings.


#include "FurnitureFieldSystemActor.h"
#include "Field\FieldSystemComponent.h"

AFurnitureFieldSystemActor::AFurnitureFieldSystemActor()
{
    
}

void AFurnitureFieldSystemActor::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("Begin Play"));  
}

void AFurnitureFieldSystemActor::ApplyField(float Magnitude)
{
    if(FieldSystemComponent)
    {
        RadialFalloff = NewObject<URadialFalloff>();
        RadialFalloff->SetRadialFalloff(Magnitude, 1, 1, 0, 30, GetActorLocation(), EFieldFalloffType::Field_Falloff_Linear);
        FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
        DrawDebugSphere(GetWorld(), RadialFalloff->Position, 100, 100, FColor::Red);
    }
}
