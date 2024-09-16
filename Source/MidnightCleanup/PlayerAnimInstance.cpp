// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Net/UnrealNetwork.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Speed = 0.f;
	Direction = 0.f;
	bIsFalling = false;
	bIsMove = false;
	bIsHeld = false;
	bIsInteractive = false;
	bIsLeftClickStart = false;
	bIsLeftClicking = false;
	bIsAnimating = false;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* Owner = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Owner)
	{
		Direction = UKismetAnimationLibrary::CalculateDirection(Owner->GetCharacterMovement()->Velocity, Owner->GetControlRotation());
		Velocity = Owner->GetCharacterMovement()->Velocity;
		Speed = Owner->GetCharacterMovement()->Velocity.Size2D();
		bIsFalling = Owner->GetCharacterMovement()->IsFalling();

		FVector CurrentAcceleration = Owner->GetCharacterMovement()->GetCurrentAcceleration();
		bool bIsAccelerating = CurrentAcceleration.SizeSquared() > 0.f;

		if (bIsAccelerating)
		{
			bIsMove = true;
		}
		else
		{
			bIsMove = false;
		}

		bIsHeld = Owner->bIsHeld;
		bIsInteractive = Owner->bIsInteractive;
		bIsMopHeld = Owner->bIsMopHeld;
		bIsAxeHeld = Owner->bIsAxeHeld;
		bIsSpongeHeld = Owner->bIsSpongeHeld;
		bIsErasing = Owner->bIsErasing;
		bIsEraseStart = Owner->bIsEraseStart;
		bIsFlashlightHeld = Owner->bIsFlashlightHeld;
		bIsLeftClickStart = Owner->bIsLeftClickStart;
		bIsLeftClicking = Owner->bIsLeftClicking;
	}
}

void UPlayerAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAnimInstance, bIsAnimating);
}




