// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components\SkeletalMeshComponent.h"
#include "Components\CapsuleComponent.h"
#include "Components\WidgetInteractionComponent.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "PC.h"
#include "InteractiveObject.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CustomDecal.h"
#include "Seunggi\BasicInteractionObject.h"
#include "Seunggi\InteractionPickUpObject.h"
#include "Mop.h"
#include "GI.h"
#include "InteractionBox.h"
#include "InteractionTrash.h"
#include "Flashlight.h"
#include "Seunggi/InteractionDoor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Seunggi/LargePickUpObject.h"
#include "Sponge.h"
#include "OpacityDecal.h"	
#include "TextureDecal.h"	
#include "Algo/Sort.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "InteractionFurniture.h"
#include "InteractionBucket.h"
#include "Seunggi/InteractionFaucet.h"
#include "InteractionKeypad.h"
#include "Seunggi/InGameGM.h"
#include "Seunggi\ObjectOrganize.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator(0, -90.f, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(RootComponent);

	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FPSMesh->SetupAttachment(Camera);

	GetCharacterMovement()->SetCrouchedHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());

	//GetCharacterMovement()->SetIsReplicated(true);

	Sight = CreateDefaultSubobject<USceneComponent>(TEXT("Sight"));
	Sight->SetupAttachment(RootComponent);

	PreviousMousePos = FVector2D::ZeroVector;
	CurrentMousePos = FVector2D::ZeroVector;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	Trace3DWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Trace3DWidget"));

	if (Trace3DWidget)
	{
		Trace3DWidget->SetupAttachment(Camera);
		Trace3DWidget->InteractionDistance = 500.f;
		Trace3DWidget->bShowDebug = true;
	}

	Inventory.SetNum(4);
	CurrentInventoryIndex = 0;
	PreviousInventoryIndex = Inventory.Num() - 1;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (IsLocallyControlled())
	{
		FPSMesh->SetVisibility(true);
		GetMesh()->SetVisibility(false);

		//GetMesh()->SetVisibility(true);

		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);
			GrabbedObjectLocation = TraceEnd;
		}
	}
	else
	{
		FPSMesh->SetVisibility(false);
		GetMesh()->SetVisibility(true);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HighlightedActor)
	{
		HighlightedActor->DrawOutline(false);
	}
	HighlightedActor = nullptr;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsLocallyControlled())
	{
		GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 0, 0), GetActorRotation(), true);
		GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, 0, 0), GetActorRotation(), true);
		if (PC)
		{
			APC* TempPC = Cast<APC>(PC);
			if (TempPC)
			{
				TempPC->HideFurnitureProgressWidget();
			}

			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

			TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			//Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Destructible));
			//Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

			bool Result;
			FHitResult OutHit;
			UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (GI->bDrawDebug)
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
			}
			else
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
			}

			if (Result)
			{
				// InteractiveObject
				ABasicInteractionObject* Object = Cast<ABasicInteractionObject>(OutHit.GetActor());
				if (Object)
				{
					AInteractionKeypad* Temp = Cast<AInteractionKeypad>(OutHit.GetActor());
					if (Temp)
					{
						Keypad = Temp;
					}

					if (bIsHeld)
					{
						if (Cast<AObjectOrganize>(Object))
						{
							if (HighlightedActor != nullptr && HighlightedActor != Object)
							{
								HighlightedActor->DrawOutline(false);
							}
							HighlightedActor = Object;
						}
						ABasicTool* Tool = Cast<ABasicTool>(OwnedActor);
						if (Tool)
						{
							switch (Tool->GetToolType())
							{
							case EToolType::ToolType_Mop:
							{
								if (Object->ObjectType == EObjectType::ObjectType_PickUp)
								{
									AInteractionPickUpObject* PickUpObject = Cast<AInteractionPickUpObject>(Object);
									if (PickUpObject && PickUpObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Bucket)
									{
										if (HighlightedActor != nullptr && HighlightedActor != Object)
										{
											HighlightedActor->DrawOutline(false);
										}
										HighlightedActor = Object;
									}
								}
							}
							break;
							case EToolType::ToolType_Axe:
							{
								if (Object->ObjectType == EObjectType::ObjectType_Furniture)
								{
									if (TempPC)
									{
										TempPC->ShowFurnitureProgressWidget();

										AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(Object);
										TempPC->UpdateFurnitureProgressBar(Furniture->MaxHp, Furniture->CurrentHp);
									}

									if (HighlightedActor != nullptr && HighlightedActor != Object)
									{
										HighlightedActor->DrawOutline(false);
									}
									HighlightedActor = Object;

								}
							}
							break;
							case EToolType::ToolType_Sponge:
							{
								if (Object->ObjectType == EObjectType::ObjectType_PickUp)
								{
									AInteractionPickUpObject* PickUpObject = Cast<AInteractionPickUpObject>(Object);
									if (PickUpObject && PickUpObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Bucket)
									{
										if (HighlightedActor != nullptr && HighlightedActor != Object)
										{
											HighlightedActor->DrawOutline(false);
										}
										HighlightedActor = Object;
									}
								}
							}
							break;
							}
						}
						else if (Cast<AInteractionBucket>(OwnedActor))
						{
							AInteractionFaucet* Faucet = Cast<AInteractionFaucet>(Object);
							if (Faucet)
							{
								if (HighlightedActor != nullptr && HighlightedActor != Object)
								{
									HighlightedActor->DrawOutline(false);
								}
								HighlightedActor = Object;
							}
						}
					}
					else
					{
						AInteractionFaucet* Faucet = Cast<AInteractionFaucet>(Object);
						if (Faucet)
						{
							return;
						}
						if (HighlightedActor != nullptr && HighlightedActor != Object)
						{
							HighlightedActor->DrawOutline(false);
						}
						if (Object->ObjectType != EObjectType::ObjectType_Furniture)
						{
							HighlightedActor = Object;
						}
					}
				}
				else if (Object == nullptr)
				{
					if (HighlightedActor)
					{
						HighlightedActor->DrawOutline(false);
						HighlightedActor = nullptr;
					}
				}
			}
			else
			{
				if (HighlightedActor)
				{

				}
			}
			if (HighlightedActor)
			{
				HighlightedActor->DrawOutline(true);
			}


			if (PhysicsHandle->GrabbedComponent && PhysicsHandle->GrabbedComponent->GetOwner())
			{
				ABasicTool* Tool = Cast<ABasicTool>(PhysicsHandle->GrabbedComponent->GetOwner());

				if (Tool && Tool->GetToolType() == EToolType::ToolType_Sponge)
				{
					if (bFixView)
					{
						FVector WorldLocation, WorldDirection;
						PC->DeprojectScreenPositionToWorld(CurrentMousePos.X, CurrentMousePos.Y, WorldLocation, WorldDirection);

						FVector CameraPos;
						FRotator CameraRot;
						PC->GetPlayerViewPoint(CameraPos, CameraRot);
						bool SpongeResult;
						TArray<TEnumAsByte<EObjectTypeQuery>> StaticObjects;
						StaticObjects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
						TraceEnd = CameraPos + (WorldDirection * 100000);
						if (GI->bDrawDebug)
						{
							SpongeResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, StaticObjects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
						}
						else
						{
							SpongeResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, StaticObjects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
						}

						if (SpongeResult)
						{
							float ImpactPointLength = (OutHit.ImpactPoint - CameraPos).Length();

							FVector TargetLocation = CameraPos + (WorldDirection * ImpactPointLength);
							C2S_SetObjectLocation(TargetLocation);

							FRotator NewRotation = FRotator(0.f, 90.f, 90.f);
							C2S_SetObjectRotation(NewRotation);
						}

					}
					else
					{
						FVector Location = FPSMesh->GetSocketLocation(FName("Sponge"));
						FRotator Rotation = FPSMesh->GetSocketRotation(FName("Sponge"));

						C2S_SetObjectLocation(Location);
						C2S_SetObjectRotation(Rotation);
					}
				}
			}
		}
	}
}

// Called to bind functionality to inputCurrentPos
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		UIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpStart);
		UIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnd);
		UIC->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		UIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
		UIC->BindAction(TapAction, ETriggerEvent::Started, this, &APlayerCharacter::Tab);
		UIC->BindAction(TapAction, ETriggerEvent::Completed, this, &APlayerCharacter::TabEnd);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Started, this, &APlayerCharacter::LeftClickStart);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftClick);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Canceled, this, &APlayerCharacter::LeftClickCanceled);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &APlayerCharacter::LeftClickEnd);
		UIC->BindAction(InteractiveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interaction, true);
		//UIC->BindAction(InteractiveAction, ETriggerEvent::Completed, this, &APlayerCharacter::Interaction, false);
		UIC->BindAction(LeftMouseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftMouse);
		UIC->BindAction(DropAction, ETriggerEvent::Started, this, &APlayerCharacter::DropObject);
		UIC->BindAction(FlashlightAction, ETriggerEvent::Started, this, &APlayerCharacter::SetFlashlight);
		UIC->BindAction(ToolDurabilityAction, ETriggerEvent::Started, this, &APlayerCharacter::SetToolMaxDurability);
		UIC->BindAction(MouseWheelAction, ETriggerEvent::Started, this, &APlayerCharacter::Wheel);
		UIC->BindAction(EscAction, ETriggerEvent::Started, this, &APlayerCharacter::DisableKeypad);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	if (!bFixView)
	{
		FVector2D Data = Value.Get<FVector2D>();

		AddControllerYawInput(Data.X);
		AddControllerPitchInput(Data.Y);

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (IsLocallyControlled())
		{
			GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 0, 0), GetActorRotation(), true);
			GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, 0, 0), GetActorRotation(), true);
			if (PC)
			{
				int32 ScreenSizeX;
				int32 ScreenSizeY;
				FVector CrosshairWorldPosition; //3D
				FVector CrosshairWorldDirection; //3D 

				FVector CameraLocation;
				FRotator CameraRotation;

				PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
				PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

				PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

				FVector TraceStart = CameraLocation;
				FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

				if (PhysicsHandle->GrabbedComponent)
				{
					float DeltaTime = GetWorld()->GetDeltaSeconds();
					float Speed = 10.0f;
					FVector StartLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();


					ALargePickUpObject* LargeObject = Cast<ALargePickUpObject>(PhysicsHandle->GrabbedComponent->GetOwner());
					if (LargeObject)
					{

						FVector CurrentLocation = GetActorLocation();
						FVector MinRange = LargeObject->GetActorLocation() - 200;
						FVector MaxRange = LargeObject->GetActorLocation() + 200;
						FVector GrabLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();
						if (LargeObject->Player1 && LargeObject->Player2)
						{
							TraceEnd = GetActorLocation() + (GetActorForwardVector() * 100);
							FVector NewLocation = (LargeObject->Player1->GetActorLocation() + LargeObject->Player2->GetActorLocation()) / 2;
							NewLocation = NewLocation + PhysicsHandle->GrabbedComponent->GetSocketLocation(PhysicsHandle->GrabbedBoneName);
							float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
							if (Distance > 500)
							{
								DropObject();
							}
							C2S_SetObjectLocation(TraceEnd);
						}
						else //if (LargeObject->Player1)
						{
							TraceEnd = TraceStart + (CrosshairWorldDirection * 200);
							float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();

							if (Distance > 500)
							{
								DropObject();
							}
							C2S_SetObjectLocation(TraceEnd);

						}
					}
					else
					{
						ASponge* Sponge = Cast<ASponge>(PhysicsHandle->GrabbedComponent->GetOwner());
						if (Sponge)
						{
							return;
						}

						// Move the grabbed component to the desired location
						TraceEnd = TraceStart + (CrosshairWorldDirection * DistanceLength);
						float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
						FVector ResultLocation = TraceEnd;
						if (Distance > DistanceLength+10)
						{
							ResultLocation = FMath::VInterpTo(StartLocation, TraceEnd, DeltaTime, Speed);
						}
						else
						{
							C2S_SetInterpolationSpeed(100.f);
							ResultLocation = TraceEnd;
						}
						if (Distance > 500)
						{
							DropObject();
						}
						//TraceEnd = GetMesh()->GetBoneLocation(TEXT("Box"))+GetActorLocation();
						//PhysicsHandle->SetTargetLocation(TraceEnd);
						C2S_SetObjectLocation(ResultLocation);
						FTransform Transform = FTransform(FRotator::ZeroRotator) * GetTransform();
						C2S_SetObjectRotation(Transform.Rotator());
					}
				}
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Data = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator ForwardRotation = FRotator(0, Rotation.Yaw, 0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ForwardRotation);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(ForwardRotation);
	AddMovementInput(ForwardVector, Data.Y);
	AddMovementInput(RightVector, Data.X);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (IsLocallyControlled())
	{
		GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 0, 0), GetActorRotation(), true);
		GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, 0, 0), GetActorRotation(), true);
		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

			if (PhysicsHandle->GrabbedComponent)
			{
				float DeltaTime = GetWorld()->GetDeltaSeconds();
				float Speed = 10.0f;
				FVector StartLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();


				ALargePickUpObject* LargeObject = Cast<ALargePickUpObject>(PhysicsHandle->GrabbedComponent->GetOwner());
				if (LargeObject)
				{

					FVector CurrentLocation = GetActorLocation();
					FVector MinRange = LargeObject->GetActorLocation() - 200;
					FVector MaxRange = LargeObject->GetActorLocation() + 200;
					FVector GrabLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();
					if (LargeObject->Player1 && LargeObject->Player2)
					{
						TraceEnd = GetActorLocation() + (GetActorForwardVector() * 100);
						FVector NewLocation = (LargeObject->Player1->GetActorLocation() + LargeObject->Player2->GetActorLocation()) / 2;
						NewLocation = NewLocation + PhysicsHandle->GrabbedComponent->GetSocketLocation(PhysicsHandle->GrabbedBoneName);
						float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
						if (Distance > 500)
						{
							DropObject();
						}
						C2S_SetObjectLocation(TraceEnd);
					}
					else //if (LargeObject->Player1)
					{
						TraceEnd = TraceStart + (CrosshairWorldDirection * 200);
						float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();

						if (Distance > 500)
						{
							DropObject();
						}
						C2S_SetObjectLocation(TraceEnd);

					}
				}
				else
				{
					ASponge* Sponge = Cast<ASponge>(PhysicsHandle->GrabbedComponent->GetOwner());
					if (Sponge)
					{
						return;
					}

					// Move the grabbed component to the desired location
					TraceEnd = TraceStart + (CrosshairWorldDirection * DistanceLength);
					float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
					FVector ResultLocation;
					if (Distance > DistanceLength+10)
					{
						ResultLocation = FMath::VInterpTo(StartLocation, TraceEnd, DeltaTime, Speed);
					}
					else
					{
						ResultLocation = TraceEnd;
					}
					if (Distance > 500)
					{
						DropObject();
					}
					//TraceEnd = GetMesh()->GetBoneLocation(TEXT("Box"))+GetActorLocation();
					//PhysicsHandle->SetTargetLocation(TraceEnd);
					C2S_SetObjectLocation(ResultLocation);
					FTransform Transform = FTransform(FRotator::ZeroRotator) * GetTransform();
					C2S_SetObjectRotation(Transform.Rotator());

				}
			}
		}
	}
}

void APlayerCharacter::Sprint()
{
	//Client
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	//Server
	C2S_SetSprint(true);
}

void APlayerCharacter::StopSprint()
{
	//Client
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//Server
	C2S_SetSprint(false);
}

void APlayerCharacter::LeftMouse(const FInputActionValue& Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	ABasicTool* CurrentTool = Cast<ABasicTool>(FPSOwnedActor);

	if (bFixView && CurrentTool)
	{
		if (CurrentTool->GetToolType() == EToolType::ToolType_Sponge)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			if (PC)
			{
				int32 ScreenSizeX;
				int32 ScreenSizeY;
				FVector CrosshairWorldPosition; //3D
				FVector CrosshairWorldDirection; //3D 

				FVector CameraLocation;
				FRotator CameraRotation;

				PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
				PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

				PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

				FVector TraceStart = CameraLocation;
				FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

				TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

				Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

				FHitResult OutHit;
				bool Result;
				UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (GI->bDrawDebug)
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
				}
				else
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
				}

				ASponge* Sponge = Cast<ASponge>(CurrentTool);
				if (Sponge)
				{
					PC->GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);
					if (Sponge->Durability > 0)
					{
						if (Result)
						{
							AOpacityDecal* Decal = Cast<AOpacityDecal>(OutHit.GetActor());

							float MouseDistance = FVector2D::Distance(CurrentMousePos, PreviousMousePos);
							if (Decal)
							{
								if (MouseDistance > Decal->MinLegnth)
								{
									if (Decal->CurrentOpacity <= 0)
									{
										C2S_SetErasingState(false);
										C2S_SetCurrentDecal(nullptr);
									}
									else
									{
										C2S_SetErasingState(true);
										C2S_SetCurrentDecal(Decal);
										C2S_DecreaseDurability(Sponge);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		C2S_SetErasingState(false);
	}
}

void APlayerCharacter::SetInventory(float Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	C2S_SetInventory(Value);
}

void APlayerCharacter::SetFlashlight()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	ABasicTool* CurrentTool = Cast<ABasicTool>(Inventory[CurrentInventoryIndex]);
	if (CurrentTool)
	{
		if (CurrentTool->GetToolType() == EToolType::ToolType_Flashlight)
		{
			AFlashlight* Flashlight = Cast<AFlashlight>(Inventory[CurrentInventoryIndex]);
			if (Flashlight->bIsActivate)
			{
				C2S_OnFlashlight(Flashlight, false);
			}
			else
			{
				C2S_OnFlashlight(Flashlight, true);
			}
		}
	}
}

void APlayerCharacter::SetToolMaxDurability()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	ABasicTool* Tool = Cast<ABasicTool>(Inventory[CurrentInventoryIndex]);
	C2S_SetToolMaxDurability(Tool);
}

void APlayerCharacter::SortDecalsBySortOrder(TArray<class AActor*>& Decals)
{
	Algo::Sort(Decals, [](AActor* A, AActor* B)
		{
			ATextureDecal* DecalA = Cast<ATextureDecal>(A);
			ATextureDecal* DecalB = Cast<ATextureDecal>(B);
			return DecalA->GetSortOrder() < DecalB->GetSortOrder();
		});
}

void APlayerCharacter::Axe_Hit()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (bIsAxeHeld)
	{
		AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(HighlightedActor);
		if (Furniture)
		{
			UE_LOG(LogTemp, Warning, TEXT("Furniture Interaction"));
			C2S_Axe_Hit(Furniture);
		}
	}
}

void APlayerCharacter::EndAxeWipe()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (bIsAxeHeld)
	{
		AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(HighlightedActor);
		if (Furniture)
		{
			DecreaseFurnitureDurability((float)(Furniture->MaxHp), (float)(Furniture->CurrentHp));
		}
		else
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DecreaseFurnitureDurability(5.0f, 0.0f);
		}
	}
}

void APlayerCharacter::Wheel(const FInputActionValue& Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	float Data = Value.Get<float>();

	SetInventory(Data);

	UE_LOG(LogTemp, Warning, TEXT("Previous : %d, Current : %d"), PreviousInventoryIndex, CurrentInventoryIndex);
}

void APlayerCharacter::EnableKeypad()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->ShowRewardWidget(this);
	}
}

void APlayerCharacter::DisableKeypad()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->IsEnableRewardWidget())
	{
		PC->HideRewardWidget();
		PC->C2S_OpenInitGame();
	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, bIsInteractive);
	DOREPLIFETIME(APlayerCharacter, bIsLeftClickStart);
	DOREPLIFETIME(APlayerCharacter, bIsLeftClicking);
	DOREPLIFETIME(APlayerCharacter, bIsErasing);
	DOREPLIFETIME(APlayerCharacter, bIsHeld);
	DOREPLIFETIME(APlayerCharacter, bIsHeldBox);
	DOREPLIFETIME(APlayerCharacter, bIsMopHeld);
	DOREPLIFETIME(APlayerCharacter, bIsAxeHeld);
	DOREPLIFETIME(APlayerCharacter, bIsFlashlightHeld);
	DOREPLIFETIME(APlayerCharacter, bIsSpongeHeld);
	DOREPLIFETIME(APlayerCharacter, bFixView);
	DOREPLIFETIME(APlayerCharacter, bUpdateInventoryByUI);
	DOREPLIFETIME(APlayerCharacter, bIsChangePreviousIndex);
	DOREPLIFETIME(APlayerCharacter, bIsChangeCurrentIndex);
	DOREPLIFETIME(APlayerCharacter, bIsEraseStart);
	DOREPLIFETIME(APlayerCharacter, CurrentInventoryIndex);
	DOREPLIFETIME(APlayerCharacter, PreviousInventoryIndex);
	DOREPLIFETIME(APlayerCharacter, OwnedActor);
	DOREPLIFETIME(APlayerCharacter, FPSOwnedActor);
	DOREPLIFETIME(APlayerCharacter, ActorsToIgnore);
	DOREPLIFETIME(APlayerCharacter, DistanceLength);
	DOREPLIFETIME(APlayerCharacter, Keypad);
}

void APlayerCharacter::JumpStart()
{
	Super::Jump();
	bIsJump = true;
}

void APlayerCharacter::JumpEnd()
{
	bIsJump = false;
}

void APlayerCharacter::LeftClickStart()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	Trace3DWidget->PressPointerKey(EKeys::LeftMouseButton);

	C2S_SetLeftClickStart(true);
	C2S_SetLeftClicking(false);

	ASponge* Sponge = Cast<ASponge>(FPSOwnedActor);
	if (Sponge)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

			TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

			FHitResult OutHit;
			bool Result;
			UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI->bDrawDebug)
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}
			else
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}

			if (Result)
			{
				AInteractionBucket* Bucket = Cast<AInteractionBucket>(OutHit.GetActor());
				if (Bucket)
				{
					UE_LOG(LogTemp, Warning, TEXT("Cleaning"));
					C2S_CleaningTool(Bucket);
				}
				
				AOpacityDecal* Decal = Cast<AOpacityDecal>(OutHit.GetActor());
				if (Decal)
				{
					ABasicTool* Tool = Cast<ABasicTool>(FPSOwnedActor);
					if (Tool->GetToolType() == EToolType::ToolType_Sponge)
					{
						Tool->PickUpFPSSponge(this);
					}

					C2S_SetFixViewState(true);

					PC->SetMouseLocation(ScreenSizeX / 2, ScreenSizeY / 2);

					PreviousMousePos.X = ScreenSizeX / 2;
					PreviousMousePos.Y = ScreenSizeY / 2;

					CurrentMousePos.X = ScreenSizeX / 2;
					CurrentMousePos.Y = ScreenSizeY / 2;
				}
			}
		}

		C2S_SetSponge(Sponge, true);
	}

}

void APlayerCharacter::C2S_SetSpongeOverlappedDecal_Implementation(class AOpacityDecal* OverlappedDecal)
{
	if (FPSOwnedActor)
	{
		ASponge* Sponge = Cast<ASponge>(FPSOwnedActor);

		if (Sponge)
		{
			Sponge->S2A_SetOverlappedDecal(OverlappedDecal);
		}
	}
}

void APlayerCharacter::C2S_SetToolMaxDurability_Implementation(ABasicTool* Tool)
{
	if (Tool)
	{
		switch (Tool->GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			AMop* Mop = Cast<AMop>(Tool);
			if (Mop)
			{
				Mop->C2S_SetMaxDurability();
			}
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			ASponge* Sponge = Cast<ASponge>(Tool);
			if (Sponge)
			{
				Sponge->C2S_SetMaxDurability();
			}
		}
		break;
		}
	}
}

void APlayerCharacter::C2S_SetErasingState_Implementation(bool State)
{
	S2A_SetErasingState(State);
}

void APlayerCharacter::S2A_SetErasingState_Implementation(bool State)
{
	bIsErasing = State;
}

void APlayerCharacter::Interaction(bool State)
{
	if (!IsLocallyControlled())
	{
		return;
	}
	
	C2S_SetInteractiveState(State);

	if (HighlightedActor)
	{
		C2S_Interaction(HighlightedActor);
		HighlightedActor->DrawOutline(false);
		AInteractionPickUpObject* PickUpObject = Cast<AInteractionPickUpObject>(HighlightedActor);
		if (PickUpObject)
		{
			if (!PickUpObject->ObjectOwner)
			{
				C2S_SetInterpolationSpeed(30.0f);

			}

			if (!bIsHeld)
			{
				UE_LOG(LogTemp, Warning, TEXT("Interaction"));
				C2S_AddActorsToIgnore(Cast<AInteractionPickUpObject>(HighlightedActor));
				//C2S_Interaction(HighlightedActor);
				C2S_SetHeldState(true);
				C2S_SetOwnedActor(Cast<AInteractionPickUpObject>(HighlightedActor));
			}
		}

		HighlightedActor = nullptr;
	}
}

void APlayerCharacter::C2S_Interaction_Implementation(ABasicInteractionObject* InteractiveObject)
{
	if (IsValid(InteractiveObject))
	{
		AInteractionBucket* LargeObject = Cast<AInteractionBucket>(InteractiveObject);
		AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(InteractiveObject);
		ABasicTool* Pickup = Cast<ABasicTool>(InteractiveObject);
		if (Pickup)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FTransform Transform = FTransform(GetActorLocation());
			AInteractionPickUpObject* object = GetWorld()->SpawnActor<ABasicTool>(Pickup->GetClass(), SpawnParam);
			C2S_AddActorsToIgnore(object);
			object->Box->SetSimulatePhysics(false);
			FPSOwnedActor = object;
			FPSOwnedActor->SetObjectOwner(this);

			OnRep_SetFPSObject();
		}
		if (LargeObject)
		{
			LargeObject->InterAction(this);
		}
		else if (Furniture)
		{
			Furniture->InterAction(this);
		}
		else
		{
			InteractiveObject->InterAction(this);
			UE_LOG(LogTemp, Warning, TEXT("Interaction"));
		}
	}

	if (PhysicsHandle->GetGrabbedComponent())
	{
		FName objectname = PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetFName();
		UE_LOG(LogTemp, Warning, TEXT("Grabbed Object : %s"), *objectname.ToString());
	}

}

void APlayerCharacter::C2S_AttachTool_Implementation(ABasicInteractionObject* InteractiveObject)
{
	ABasicTool* Tool = Cast<ABasicTool>(InteractiveObject);
	if (Tool)
	{
		Tool->AttachPreviousSocket(this);
	}
}

void APlayerCharacter::C2S_DetachTool_Implementation(ABasicInteractionObject* InteractiveObject)
{
	ABasicTool* Tool = Cast<ABasicTool>(InteractiveObject);
	if (Tool)
	{
		Tool->DetachPreviousSocket(this);

		ABasicTool* Pickup = Cast<ABasicTool>(InteractiveObject);
		if (Pickup)
		{
			FAttachmentTransformRules AttachmentRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				true
			);
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FTransform Transform = FTransform(GetActorLocation());
			AInteractionPickUpObject* object = GetWorld()->SpawnActor<ABasicTool>(Pickup->GetClass(), SpawnParam);
			C2S_AddActorsToIgnore(object);
			object->Box->SetSimulatePhysics(false);
			FPSOwnedActor = object;

			OnRep_SetFPSObject();

			Pickup->SetObjectOwner(this);

			S2A_PickUpFPSObject(Pickup);
		}

		//C2S_Interaction(Tool);
	}
}

void APlayerCharacter::C2A_PickUp_Implementation(ABasicInteractionObject* InteractiveObject)
{

}

void APlayerCharacter::C2S_SetInteractiveState_Implementation(bool State)
{
	bIsInteractive = State;
}

void APlayerCharacter::DropObject()
{
	if (!IsLocallyControlled())
	{
		//return;
	}

	C2S_SetFixViewState(false);

	if (OwnedActor)
	{
		C2S_SetHeldState(false);
		C2S_RemoveActorsToIgnore(OwnedActor);
		C2S_DropObject(OwnedActor);
		C2S_SetOwnedActor(nullptr);
		//HighlightedActor = nullptr;
	}
}

void APlayerCharacter::C2S_DropObject_Implementation(AInteractionPickUpObject* InteractiveObject)
{
	if (IsValid(InteractiveObject))
	{
		InteractiveObject->DropObject(this);
	}
}

void APlayerCharacter::C2S_DropPreviousObject_Implementation(AInteractionPickUpObject* InteractiveObject)
{
	if (IsValid(InteractiveObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("C2S_DropObject_Implementation"));
		InteractiveObject->DropPreviousObject(this);
	}
}

void APlayerCharacter::C2S_SetHeldState_Implementation(bool State)
{
	S2A_SetHeldState(State);
}

void APlayerCharacter::S2A_SetHeldState_Implementation(bool State)
{
	bIsHeld = State;
}

void APlayerCharacter::C2S_SetInventory_Implementation(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("C2S_SetInventory_Implementation"));

	if (!bUpdateInventoryByUI)
	{
		int32 CurrentTemp = CurrentInventoryIndex;
		int32 PreviousTemp = PreviousInventoryIndex;

		if (Value < 0)
		{
			if (CurrentInventoryIndex < Inventory.Num() - 1)
			{
				CurrentInventoryIndex++;
			}
			else if (CurrentInventoryIndex == Inventory.Num() - 1)
			{
				CurrentInventoryIndex = 0;
			}
		}
		else
		{
			if (CurrentInventoryIndex > 0)
			{
				CurrentInventoryIndex--;
			}
			else if (CurrentInventoryIndex == 0)
			{
				CurrentInventoryIndex = Inventory.Num() - 1;
			}
		}

		if (CurrentTemp != CurrentInventoryIndex)
		{
			PreviousInventoryIndex = CurrentTemp;
			if (PreviousTemp != PreviousInventoryIndex)
			{
				S2A_SetPreviousInventoryChangeState(true);
			}
			else
			{
				S2A_SetPreviousInventoryChangeState(false);
			}
			S2A_SetCurrentInventoryChangeState(true);
		}
		else
		{
			S2A_SetCurrentInventoryChangeState(false);
			S2A_SetPreviousInventoryChangeState(false);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("PlayerInventoryIndex : Cur(%d), Prev(%d)"), CurrentInventoryIndex, PreviousInventoryIndex)
		AInteractionPickUpObject* PreviousObject = Inventory[PreviousInventoryIndex];
	if (PreviousObject)
	{
		ABasicTool* PreviousTool = Cast<ABasicTool>(Inventory[PreviousInventoryIndex]);
		if (PreviousTool && bIsChangePreviousIndex)
		{
			switch (PreviousTool->GetToolType())
			{
			case EToolType::ToolType_Sponge:
			{
				ASponge* Sponge = Cast<ASponge>(PreviousTool);
				if (Sponge)
				{
					C2S_SetSponge(Sponge, false);
					C2S_SetCurrentDecal(nullptr);
				}
			}
			break;
			case EToolType::ToolType_Mop:
			{
				AMop* Mop = Cast<AMop>(PreviousTool);
				if (Mop)
				{
					C2S_SetMop(Mop, false);
					C2S_SetCurrentDecal(nullptr);
				}
			}
			break;
			case EToolType::ToolType_Axe:
			{
				APlayerController* PC = Cast<APlayerController>(GetController());
				if (PC)
				{
					APC* TempPC = Cast<APC>(PC);
					if (TempPC)
					{
						TempPC->HideFurnitureProgressWidget();
					}
				}
			}
			break;
			}
			C2S_AttachTool(PreviousObject);
			PreviousTool->VisibleObject(false);
			C2S_SetHeldState(false);
			C2S_SetOwnedActor(nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Previous Tool Remove"));
		}
		else
		{
			if (OwnedActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("DropObject"));
				C2S_SetHeldState(false);
				C2S_RemoveActorsToIgnore(OwnedActor);
				C2S_DropPreviousObject(OwnedActor);
				C2S_SetOwnedActor(nullptr);
				HighlightedActor = nullptr;
			}

			UE_LOG(LogTemp, Warning, TEXT("Previous Object Remove"));
		}
	}

	AInteractionPickUpObject* CurrentObject = Inventory[CurrentInventoryIndex];
	if (CurrentObject && bIsChangeCurrentIndex)
	{
		if (CurrentObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Tool)
		{
			if (CurrentObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Tool)
			{
				ABasicTool* CurrentTool = Cast<ABasicTool>(Inventory[CurrentInventoryIndex]);
				if (CurrentTool)
				{
					switch (CurrentTool->GetToolType())
					{
					case EToolType::ToolType_Sponge:
					{
						ASponge* Sponge = Cast<ASponge>(CurrentTool);
						if (Sponge)
						{
							C2S_SetSponge(Sponge, true);
							C2S_SetCurrentDecal(CurrentSpongeDecal);
						}
					}
					break;
					case EToolType::ToolType_Mop:
					{
						AMop* Mop = Cast<AMop>(CurrentTool);
						if (Mop)
						{
							C2S_SetMop(Mop, true);
							C2S_SetCurrentDecal(CurrentMopDecal);
						}
					}
					break;
					}

					C2S_DetachTool(CurrentTool);
					//CurrentTool->VisibleObject(true);
					UE_LOG(LogTemp, Warning, TEXT("Current Tool Add"));
				}
			}

			C2S_SetHeldState(true);
			C2S_SetOwnedActor(CurrentObject);
		}
	}
}

void APlayerCharacter::C2S_SetSponge_Implementation(ASponge* Sponge, bool State)
{
	if (IsValid(Sponge))
	{
		Sponge->S2A_SetSpongeActivate(State);
	}
}

void APlayerCharacter::C2S_SetMop_Implementation(AMop* Mop, bool State)
{
	if (IsValid(Mop))
	{

		Mop->S2A_SetMopActivate(State);
	}
}

void APlayerCharacter::C2S_SetObjectLocation_Implementation(FVector Location)
{
	PhysicsHandle->SetTargetLocation(Location);
	S2A_SetObjectLocation(Location);

}

void APlayerCharacter::S2A_SetObjectLocation_Implementation(FVector Location)
{
	PhysicsHandle->SetTargetLocation(Location);
}

void APlayerCharacter::C2S_SetObjectRotation_Implementation(FRotator Rotation)
{
	S2A_SetObjectRotation(Rotation);
}

void APlayerCharacter::S2A_SetObjectRotation_Implementation(FRotator Rotation)
{
	PhysicsHandle->SetTargetRotation(Rotation);
}

void APlayerCharacter::C2S_SetCurrentDecal_Implementation(ACustomDecal* InDecal)
{
	S2A_SetCurrentDecal(InDecal);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentDecal"));
}

void APlayerCharacter::S2A_SetCurrentDecal_Implementation(ACustomDecal* InDecal)
{
	CurrentDecal = InDecal;

	ATextureDecal* TextureDecal = Cast<ATextureDecal>(CurrentDecal);
	AOpacityDecal* OpacityDecal = Cast<AOpacityDecal>(CurrentDecal);
	if (TextureDecal)
	{
		CurrentMopDecal = TextureDecal;
	}
	else if (OpacityDecal)
	{
		CurrentSpongeDecal = OpacityDecal;
	}
}

void APlayerCharacter::C2S_SetSprint_Implementation(bool State)
{
	bIsSprint = State;
	GetCharacterMovement()->MaxWalkSpeed = State ? SprintSpeed : WalkSpeed;
}

void APlayerCharacter::C2S_ChangeSpeed_Implementation(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void APlayerCharacter::LeftClick()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	C2S_SetLeftClickStart(false);
	C2S_SetLeftClicking(true);
}

void APlayerCharacter::LeftClickCanceled()
{
	Trace3DWidget->ReleasePointerKey(EKeys::LeftMouseButton);
}

void APlayerCharacter::LeftClickEnd()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	C2S_SetFixViewState(false);
	bIsEraseStart = false;

	C2S_SetLeftClickStart(false);
	C2S_SetLeftClicking(false);

	ABasicTool* Tool = Cast<ABasicTool>(FPSOwnedActor);
	if (Tool)
	{
		if (Tool->GetToolType() == EToolType::ToolType_Sponge)
		{
			PhysicsHandle->ReleaseComponent();
			Tool->PickUpObject(this);
			Tool->SetObjectOwner(this);

			if (IsLocallyControlled())
			{
				Tool->StaticMesh->SetVisibility(true);
			}
			else
			{
				OwnedActor->StaticMesh->SetVisibility(false);
			}

			ASponge* Sponge = Cast<ASponge>(FPSOwnedActor);
			if (Sponge)
			{
				C2S_SetSponge(Sponge, false);
			}
		}
	}
}

void APlayerCharacter::Tab()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC)
	{
		PC->HideAimWidget(this);
		PC->ShowInventoryWidget(this);
	}
}

void APlayerCharacter::TabEnd()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC)
	{
		PC->HideInventoryWidget(this);
		PC->ShowAimWidget(this);
	}
}

void APlayerCharacter::C2S_DecreaseDurability_Implementation(class ABasicTool* Tool)
{
	AMop* Mop = Cast<AMop>(Tool);
	ASponge* Sponge = Cast<ASponge>(Tool);
	if (Mop)
	{
		Mop->C2S_DecreaseDurability(CurrentMopDecal);
	}
	else if (Sponge)
	{
		Sponge->C2S_DecreaseDurability();
	}
}

void APlayerCharacter::C2S_CreateDecal_Implementation(AMop* Mop, FVector ImpactPoint, FVector Normal)
{
	if (Mop)
	{
		Mop->CreateDecal(ImpactPoint, Normal);
	}
}

void APlayerCharacter::C2S_OnFlashlight_Implementation(AFlashlight* Flashlight, bool State)
{
	if (Flashlight)
	{
		Flashlight->OnFlashlight(State);
	}
}

void APlayerCharacter::C2S_AddActorsToIgnore_Implementation(AActor* InActor)
{
	S2A_AddActorsToIgnore(InActor);
}

void APlayerCharacter::S2A_AddActorsToIgnore_Implementation(AActor* InActor)
{
	if (InActor)
	{
		ActorsToIgnore.Add(InActor);
		UE_LOG(LogTemp, Warning, TEXT("S2A_AddActorsToIgnore"));
	}
}

void APlayerCharacter::C2S_RemoveActorsToIgnore_Implementation(AInteractionPickUpObject* InActor)
{
	S2A_RemoveActorsToIgnore(InActor);
}

void APlayerCharacter::S2A_RemoveActorsToIgnore_Implementation(AInteractionPickUpObject* InActor)
{
	if (InActor)
	{
		ActorsToIgnore.Remove(InActor);
	}
}

void APlayerCharacter::C2S_SetInterpolationSpeed_Implementation(float Speed)
{
	S2A_SetInterpolationSpeed(Speed);
}

void APlayerCharacter::S2A_SetInterpolationSpeed_Implementation(float Speed)
{
	PhysicsHandle->SetInterpolationSpeed(Speed);
}

void APlayerCharacter::RemoveInventoryIndex(int Index)
{
	if (Inventory[Index])
	{
		Inventory[Index] = nullptr;
	}

}

void APlayerCharacter::AddInventoryCurrentIndex(AInteractionPickUpObject* Object)
{
	if (!Inventory[CurrentInventoryIndex])
	{
		Inventory[CurrentInventoryIndex] = Object;
	}
}

void APlayerCharacter::DropAbsolutely()
{
	if (OwnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("DropAbsolutely"));
		C2S_SetHeldState(false);
		C2S_RemoveActorsToIgnore(OwnedActor);
		C2S_DropObject(OwnedActor);
		C2S_SetOwnedActor(nullptr);
		HighlightedActor = nullptr;
	}
}

void APlayerCharacter::C2S_SetLeftClickStart_Implementation(bool State)
{
	S2A_SetLeftClickStart(State);
}

void APlayerCharacter::S2A_SetLeftClickStart_Implementation(bool State)
{
	bIsLeftClickStart = State;
}

void APlayerCharacter::C2S_SetLeftClicking_Implementation(bool State)
{
	S2A_SetLeftClicking(State);
}

void APlayerCharacter::S2A_SetLeftClicking_Implementation(bool State)
{
	bIsLeftClicking = State;
}

void APlayerCharacter::MopInteraction()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	AMop* Mop = Cast<AMop>(Inventory[CurrentInventoryIndex]);
	if (Mop)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOP DURABILITY : %d"), Mop->Durability)
			C2S_SetMop(Mop, true);

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

			TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

			TArray<FHitResult> OutHits;
			bool Result;
			UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI->bDrawDebug)
			{
				Result = UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}
			else
			{
				Result = UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}
			if (Result)
			{
				C2S_AddForce(OutHits[0].GetComponent(), ((TraceEnd - TraceStart).GetSafeNormal()) * 1000000, OutHits[0].ImpactPoint);
				
				UE_LOG(LogTemp, Warning, TEXT("Add force"));
				AInteractionBucket* Bucket = Cast<AInteractionBucket>(OutHits[0].GetActor());
				if (Bucket)
				{
					UE_LOG(LogTemp, Warning, TEXT("Cleaning"));
					C2S_CleaningTool(Bucket);
				}
			}

			if (Mop->Durability > 0)
			{
				if (Result)
				{
					int32 HighestSortOrder = -1;
					ACustomDecal* HightestSortOrderDecal = nullptr;
					for (const FHitResult& Hit : OutHits)
					{
						ACustomDecal* Decal = Cast<ACustomDecal>(Hit.GetActor());
						if (Decal && Decal->Durability > 0)
						{
							int32 CurrentSortOrder = Decal->GetSortOrder();
							if (CurrentSortOrder > HighestSortOrder)
							{
								HighestSortOrder = CurrentSortOrder;
								HightestSortOrderDecal = Decal;
							}
						}
					}

					TArray<AActor*> EraseDecals;
					if (HightestSortOrderDecal)
					{
						HightestSortOrderDecal->Box->GetOverlappingActors(EraseDecals, ATextureDecal::StaticClass());
						EraseDecals.Add(HightestSortOrderDecal);
						SortDecalsBySortOrder(EraseDecals);

						if (EraseDecals.Num() >= Mop->EraseCount)
						{
							for (int i = EraseDecals.Num() - 1; i > EraseDecals.Num() - 1 - Mop->EraseCount; i--)
							{
								ATextureDecal* EraseDecal = Cast<ATextureDecal>(EraseDecals[i]);
								if (EraseDecal)
								{
									if (EraseDecal->Durability <= 0)
									{
										C2S_SetErasingState(false);
									}
									else
									{
										if (Mop->Durability > 0)
										{
											C2S_SetErasingState(true);
											C2S_SetCurrentDecal(EraseDecal);
											C2S_DecreaseDurability(Mop);
										}
									}
								}
							}
						}
						else
						{
							for (AActor* Decal : EraseDecals)
							{
								ATextureDecal* EraseDecal = Cast<ATextureDecal>(Decal);
								if (EraseDecal->Durability <= 0)
								{
									C2S_SetErasingState(false);
								}
								else
								{
									if (Mop->Durability > 0)
									{
										C2S_SetErasingState(true);
										C2S_SetCurrentDecal(EraseDecal);
										C2S_DecreaseDurability(Mop);
									}
								}
							}
						}
					}
				}
			}
			else
			{
				FHitResult OutHit;
				TArray<TEnumAsByte<EObjectTypeQuery>> Objects1;

				Objects1.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
				Objects1.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

				if (GI->bDrawDebug)
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
				}
				else
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
				}

				if (Result)
				{
					C2S_CreateDecal(Mop, OutHit.ImpactPoint, OutHit.Normal);
				}
			}
		}
	}
}

void APlayerCharacter::C2S_CleaningTool_Implementation(class AInteractionBucket* Tool)
{
	Tool->Cleaning(this);
}

void APlayerCharacter::C2S_SetFixViewState_Implementation(bool State)
{
	S2A_SetFixViewState(State);
}

void APlayerCharacter::S2A_SetFixViewState_Implementation(bool State)
{
	bFixView = State;


}

void APlayerCharacter::C2S_SetInventoryIndex_Implementation(int32 Index)
{
	S2A_SetInventoryIndex(Index);
}

void APlayerCharacter::S2A_SetInventoryIndex_Implementation(int32 Index)
{
	if (CurrentInventoryIndex != Index)
	{
		if (PreviousInventoryIndex != CurrentInventoryIndex)
		{
			PreviousInventoryIndex = CurrentInventoryIndex;
			bIsChangePreviousIndex = true;
		}

		CurrentInventoryIndex = Index;
		bIsChangeCurrentIndex = true;
	}
	else
	{
		bIsChangeCurrentIndex = false;
		bIsChangePreviousIndex = false;
	}
}

void APlayerCharacter::C2S_SetUpdateInventoryState_Implementation(bool State)
{
	S2A_SetUpdateInventoryState(State);
}

void APlayerCharacter::S2A_SetUpdateInventoryState_Implementation(bool State)
{
	bUpdateInventoryByUI = State;
}

void APlayerCharacter::C2S_UpdatePreviousInventoryIndex_Implementation()
{
	S2A_UpdatePreviousInventoryInde();
}

void APlayerCharacter::S2A_UpdatePreviousInventoryInde_Implementation()
{
	if (CurrentInventoryIndex <= 0)
	{
		PreviousInventoryIndex = Inventory.Num() - 1;
	}
	else if (0 < CurrentInventoryIndex && CurrentInventoryIndex < Inventory.Num() - 1)
	{
		PreviousInventoryIndex = CurrentInventoryIndex - 1;
	}
	else if (CurrentInventoryIndex >= Inventory.Num() - 1)
	{
		PreviousInventoryIndex = 0;
	}
}

void APlayerCharacter::C2S_Axe_Hit_Implementation(AInteractionFurniture* Furniture)
{
	if(IsValid(Furniture))
	{
		Furniture->Axe_Hit(this);
	}
}

void APlayerCharacter::C2S_SetToolHeldState_Implementation(ABasicTool* Tool, bool State)
{
	S2A_SetToolHeldState(Tool, State);
}

void APlayerCharacter::S2A_SetToolHeldState_Implementation(ABasicTool* Tool, bool State)
{
	if (Tool)
	{
		switch (Tool->GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			bIsMopHeld = State;
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			bIsSpongeHeld = State;
		}
		break;
		case EToolType::ToolType_Axe:
		{
			bIsAxeHeld = State;
		}
		break;
		case EToolType::ToolType_Flashlight:
		{
			bIsFlashlightHeld = State;
		}
		break;
		}
	}
}

void APlayerCharacter::OnRep_SetFPSObject()
{
	if (FPSOwnedActor)
	{
		ASponge* Sponge = Cast<ASponge>(FPSOwnedActor);
		if (!Sponge)
		{
			FPSOwnedActor->Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
			FPSOwnedActor->StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		}

		AMop* Mop = Cast<AMop>(FPSOwnedActor);
		if (Mop)
		{
			if (IsLocallyControlled())
			{
				Mop->SkeletalMesh->SetVisibility(true);
			}
			else
			{
				Mop->SkeletalMesh->SetVisibility(false);
			}
		}
		else
		{
			if (IsLocallyControlled())
			{
				FPSOwnedActor->StaticMesh->SetVisibility(true);
			}
			else
			{
				FPSOwnedActor->StaticMesh->SetVisibility(false);
			}
		}
	}
		
}

void APlayerCharacter::S2A_SetCurrentInventoryChangeState_Implementation(bool State)
{
	bIsChangeCurrentIndex = State;
}

void APlayerCharacter::S2A_SetPreviousInventoryChangeState_Implementation(bool State)
{
	bIsChangePreviousIndex = State;
}

void APlayerCharacter::S2A_PickUpFPSObject_Implementation(ABasicTool* Pickup)
{
	if (Pickup)
	{
		Pickup->PickUpObject(this);
	}
}

void APlayerCharacter::C2S_AddForce_Implementation(UPrimitiveComponent* Component,FVector force, FVector Location)
{
	S2A_AddForce(Component, force, Location);
}

void APlayerCharacter::S2A_AddForce_Implementation(UPrimitiveComponent* Component, FVector force, FVector Location)
{
	if (Component)
	{
		Component->AddForceAtLocation(force, Location);
	}
}

void APlayerCharacter::OnRep_SetEraseStartState()
{
	UE_LOG(LogTemp, Warning, TEXT("MOP ERASE START"));
}

void APlayerCharacter::DecreaseFurnitureDurability(float InMaxHp, float InCurrentHp)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		APC* TempPC = Cast<APC>(PC);

		if (TempPC)
		{
			TempPC->UpdateFurnitureProgressBar(InMaxHp, InCurrentHp);
		}
	}
}

void APlayerCharacter::C2S_SetOwnedActor_Implementation(class AInteractionPickUpObject* InteractiveObject)
{
	S2A_SetOwnedActor(InteractiveObject);
}

void APlayerCharacter::S2A_SetOwnedActor_Implementation(class AInteractionPickUpObject* InteractiveObject)
{
	OwnedActor = InteractiveObject;

	ABasicTool* Tool = Cast<ABasicTool>(OwnedActor);
	if (Tool)
	{
		switch (Tool->GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			AMop* Mop = Cast<AMop>(Tool);
			AMop* NewFPSMop = Cast<AMop>(FPSOwnedActor);
			if (NewFPSMop)
			{
				NewFPSMop->S2A_SetMopDurability(Mop->Durability);
				NewFPSMop->S2A_SetMopMaterial(Mop->Durability);
			}
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			ASponge* Sponge = Cast<ASponge>(Tool);
			ASponge* NewFPSSponge = Cast<ASponge>(FPSOwnedActor);
			if (NewFPSSponge)
			{
				NewFPSSponge->S2A_SetSpongeDurability(Sponge->Durability);
				NewFPSSponge->S2A_SetSpongeMaterial(Sponge->Durability);
			}
		}
		break;
		}

	}
}