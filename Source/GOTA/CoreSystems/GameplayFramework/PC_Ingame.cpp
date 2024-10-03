// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_Ingame.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputDataAsset.h"
#include "GOTA/CoreSystems/Utility/DistanceUtils.h"
#include "GOTA/UI/Ingame/IngameUI.h"
#include "Net/UnrealNetwork.h"


void APC_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APC_Ingame, Guardian)
	DOREPLIFETIME(APC_Ingame, MouseUtils)
}

void APC_Ingame::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController()) return;
	CreateLobbyUI();
	DistanceUtils = GetWorld()->SpawnActor<ADistanceUtils>();
}

void APC_Ingame::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGuardian(Cast<AGuardian>(InPawn));
}

// -------------------------UI Stuff------------------------


// ------------------------ Guardian ------------------------

void APC_Ingame::SetGuardian(AGuardian* NewGuardian)
{
	Guardian = NewGuardian;
	GuardianChanged();
}

void APC_Ingame::GuardianChanged()
{
	if (!Guardian) return;
	if (!IsLocalController()) return;
	DistanceUtils->AttachToActor(Guardian, FAttachmentTransformRules::SnapToTargetIncludingScale);
	if (MouseUtils) Guardian->SetupGAM(MouseUtils);
}

// ---------------------- InteractionMode ----------------------

void APC_Ingame::ClickActor()
{
	IngameUI->ClickActor(MouseUtils->GetHoverActor());
}

// ----------------------- Input -----------------------

void APC_Ingame::InitInput()
{
	const UInputDataAsset* DataAsset = Cast<UInputDataAsset>(StaticLoadObject(
		UInputDataAsset::StaticClass(),
		nullptr,
		TEXT("/Game/CoreSystems/Input/DA_Input")
	));

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->AddMappingContext(DataAsset->MappingContext, 1);

	UEnhancedInputComponent* Component = Cast<UEnhancedInputComponent>(InputComponent);

	Component->BindAction(DataAsset->LeftClick, ETriggerEvent::Triggered, this, &APC_Ingame::LeftClick);

	Component->BindAction(DataAsset->Jump, ETriggerEvent::Triggered, this, &APC_Ingame::StartJump);
	Component->BindAction(DataAsset->Jump, ETriggerEvent::Canceled, this, &APC_Ingame::StopJump);
	Component->BindAction(DataAsset->Jump, ETriggerEvent::Completed, this, &APC_Ingame::StopJump);

	Component->BindAction(DataAsset->LookAround, ETriggerEvent::Triggered, this, &APC_Ingame::LookAround);
	Component->BindAction(DataAsset->ActivateLooking, ETriggerEvent::Started, this, &APC_Ingame::StartLookingAround);
	Component->BindAction(DataAsset->ActivateLooking, ETriggerEvent::Completed, this, &APC_Ingame::StopLookingAround);
}


void APC_Ingame::SetMouseUtils(AMouseUtils* NewMouseUtils)
{
	MouseUtils = NewMouseUtils;
	MouseUtilsChanged();
}

void APC_Ingame::MouseUtilsChanged()
{
	if (!IsLocalController()) return;
	MouseUtils->SetPlayerController(this);
	MouseUtils->OnHoverActorChanged.AddDynamic(this, &APC_Ingame::OnHoverActorChanged);
	if (Guardian) Guardian->SetupGAM(MouseUtils);
}

void APC_Ingame::OnHoverActorChanged(AActor* Actor)
{
	IngameUI->HoverActor(Actor);
}

void APC_Ingame::LeftClick(const FInputActionInstance& Instance)
{
	ClickActor();
}

void APC_Ingame::StartJump(const FInputActionInstance& Instance)
{
	if (Guardian) Guardian->Jump();
}

void APC_Ingame::StopJump(const FInputActionInstance& Instance)
{
	if (Guardian) Guardian->StopJumping();
}

void APC_Ingame::LookAround(const FInputActionInstance& Instance)
{
	if (!bIsLookingAround) return;
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	AddYawInput(Value.X);
	AddPitchInput(Value.Y * -1);
	SetMouseLocation(MousePositionWhenStartingLookingAround.X, MousePositionWhenStartingLookingAround.Y);
}

void APC_Ingame::StartLookingAround(const FInputActionInstance& Instance)
{
	bIsLookingAround = true;

	float MouseX = 0.0f;
	float MouseY = 0.0f;
	GetMousePosition(MouseX, MouseY);
	MousePositionWhenStartingLookingAround = FVector2D(MouseX, MouseY);
	SetShowMouseCursor(false);
}

void APC_Ingame::StopLookingAround(const FInputActionInstance& Instance)
{
	bIsLookingAround = false;
	SetShowMouseCursor(true);
}
