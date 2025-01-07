// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_Ingame.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputDataAsset.h"
#include "GOTA/CoreSystems/Guardian/Ability.h"
#include "GOTA/CoreSystems/Guardian/AbilityIndicator.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Utility/DistanceUtils.h"
#include "GOTA/CoreSystems/Utility/MouseUtils.h"
#include "GOTA/UI/Ingame/IngameUI.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ------------------------------------ Replication Setup --------------------------------------

void APC_Ingame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(APC_Ingame, BuildingPlacer, Params)
	DOREPLIFETIME_WITH_PARAMS(APC_Ingame, Guardian, Params)
	DOREPLIFETIME_WITH_PARAMS(APC_Ingame, AbilityIndicator, Params)
	DOREPLIFETIME_WITH_PARAMS(APC_Ingame, MouseUtils, Params)
}

// ---------------------------------------- Lifecycle ----------------------------------------

void APC_Ingame::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		C_Init();
	}
}

void APC_Ingame::S_Init()
{
	// Create MouseUtils
	FActorSpawnParameters MouseUtilsSpawnParams;
	MouseUtilsSpawnParams.Owner = this;
	AMouseUtils* NewMouseUtils = GetWorld()->SpawnActor<AMouseUtils>(MouseUtilsSpawnParams);
	S_SetMouseUtils(NewMouseUtils);

	// Create AbilityIndicator
	FActorSpawnParameters AbilityIndicatorSpawnParams;
	AbilityIndicatorSpawnParams.Owner = this;
	AAbilityIndicator* NewAbilityIndicator = GetWorld()->SpawnActor<AAbilityIndicator>(AbilityIndicatorSpawnParams);
	S_SetAbilityIndicator(NewAbilityIndicator);
	NewMouseUtils->AttachActorToTilePosition(NewAbilityIndicator);

	// Create BuildingPlacer
	FActorSpawnParameters BuildingPlacerSpawnParams;
	BuildingPlacerSpawnParams.Owner = this;
	ABuildingPlacer* NewBuildingPlacer = GetWorld()->SpawnActor<ABuildingPlacer>(BuildingPlacerSpawnParams);
	NewBuildingPlacer->S_Init(NewMouseUtils);
	S_SetBuildingPlacer(NewBuildingPlacer);

	ForceNetUpdate();
}

void APC_Ingame::C_Init()
{
	CreateLobbyUI();
	DistanceUtils = GetWorld()->SpawnActor<ADistanceUtils>();
}

// ---------------------------------------- Utility ----------------------------------------

void APC_Ingame::S_SetAbilityIndicator(AAbilityIndicator* NewAbilityIndicator)
{
	AbilityIndicator = NewAbilityIndicator;
	MARK_PROPERTY_DIRTY_FROM_NAME(APC_Ingame, AbilityIndicator, this)
}

// -------------------------UI Stuff------------------------


// ------------------------ Guardian ------------------------

void APC_Ingame::OnRep_Guardian()
{
	OnGuardianChanged();
}

void APC_Ingame::OnGuardianChanged()
{
	if (!Guardian)
		return;

	if (!IsLocalController())
		return;

	DistanceUtils->AttachToActor(Guardian, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void APC_Ingame::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGuardian(Cast<AGuardian>(InPawn));

	FRotator InitialRotation = FRotator(-30.0f, 0.0f, 0.0f); // Adjust these values
	SetControlRotation(InitialRotation);
}

void APC_Ingame::SetGuardian(AGuardian* NewGuardian)
{
	Guardian = NewGuardian;
	OnGuardianChanged();
	MARK_PROPERTY_DIRTY_FROM_NAME(APC_Ingame, Guardian, this)
}

// ---------------------- InteractionMode ----------------------

void APC_Ingame::ClickActor()
{
	IngameUI->ClickActor(MouseUtils->GetHoverActor());
}

void APC_Ingame::S_SetBuildingPlacer(ABuildingPlacer* NewBuildingPlacer)
{
	BuildingPlacer = NewBuildingPlacer;
	MARK_PROPERTY_DIRTY_FROM_NAME(APC_Ingame, BuildingPlacer, this)
}

void APC_Ingame::StartPlacingBuilding(UBuildingSettings* Building)
{
	BuildingPlacer->StartPlacingBuilding(Building);
}

void APC_Ingame::StopPlacingBuilding()
{
	BuildingPlacer->StopPlacingBuilding();
}

void APC_Ingame::PlaceBuilding()
{
	BuildingPlacer->PlaceBuilding();
}

// ---------------------------------------- Ability Targeting ----------------------------------------


void APC_Ingame::ActivateAbility(int32 Index)
{
	if (!Guardian) return;

	AAbility* Ability = Guardian->GetAbilityInSlot(Index-1);
	
	// Either Input was invalid or there is no skill in the selected index, either way we tried to activate
	// an ability so we should probably cancel any active targeting process
	if (!Ability)
	{
		CancelTargeting();
		return;
	}

	if (CurrentlyTargeting == Ability)
	{
		ActivateCurrentlyTargetingAbility();
	}
	else
	{
		CancelTargeting();
		StartTargeting(Ability);
	}
}

void APC_Ingame::ActivateCurrentlyTargetingAbility()
{
	if (!CurrentlyTargeting.IsValid()) return;

	CurrentlyTargeting.Get()->SRPC_ActivateAbility(MouseUtils->GetHoverAbilityTarget());
	CancelTargeting();
}

void APC_Ingame::StartTargeting(AAbility* Ability)
{
	CurrentlyTargeting = Ability;
	AbilityIndicator->Activate();
}

void APC_Ingame::CancelTargeting()
{
	CurrentlyTargeting = nullptr;
	AbilityIndicator->Deactivate();
}

// ------------------------------------------- MouseUtils -------------------------------------------

void APC_Ingame::S_SetMouseUtils(AMouseUtils* NewMouseUtils)
{
	MouseUtils = NewMouseUtils;
	MouseUtilsChanged();
	MARK_PROPERTY_DIRTY_FROM_NAME(APC_Ingame, MouseUtils, this)
}

void APC_Ingame::OnRep_MouseUtils()
{
	MouseUtilsChanged();
}

void APC_Ingame::MouseUtilsChanged()
{
	if (IsLocalController())
	{
		MouseUtils->SetPlayerController(this);
		MouseUtils->OnHoverActorChanged.AddDynamic(this, &APC_Ingame::HoverActorChanged);
	}
}

void APC_Ingame::HoverActorChanged(AActor* Actor)
{
	IngameUI->HoverActor(Actor);
}

// ------------------------------------------- Input -------------------------------------------

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

	Component->BindAction(DataAsset->BuildMenu, ETriggerEvent::Triggered, this, &APC_Ingame::ToggleBuildMenu);

	Component->BindAction(DataAsset->Ability1, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility1);
	Component->BindAction(DataAsset->Ability2, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility2);
	Component->BindAction(DataAsset->Ability3, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility3);
	Component->BindAction(DataAsset->Ability4, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility4);
	Component->BindAction(DataAsset->Ability5, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility5);
	Component->BindAction(DataAsset->Ability6, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility6);
	Component->BindAction(DataAsset->Ability7, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility7);
	Component->BindAction(DataAsset->Ability8, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility8);
}

void APC_Ingame::LeftClick(const FInputActionInstance& Instance)
{
	if (bIsLookingAround) return;
	if (CurrentlyTargeting.IsValid())
	{
		ActivateCurrentlyTargetingAbility();
	}
	else if (BuildingPlacer->IsPlacing())
	{
		PlaceBuilding();
	}
	else
	{
		ClickActor();
	}

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

void APC_Ingame::ToggleBuildMenu()
{
	if (!IngameUI) return;
	IngameUI->ToggleBuildMenu();
}

void APC_Ingame::ActivateAbility1()
{
	ActivateAbility(1);
}

void APC_Ingame::ActivateAbility2()
{
	ActivateAbility(2);
}

void APC_Ingame::ActivateAbility3()
{
	ActivateAbility(3);
}

void APC_Ingame::ActivateAbility4()
{
	ActivateAbility(4);
}

void APC_Ingame::ActivateAbility5()
{
	ActivateAbility(5);
}

void APC_Ingame::ActivateAbility6()
{
	ActivateAbility(6);
}

void APC_Ingame::ActivateAbility7()
{
	ActivateAbility(7);
}

void APC_Ingame::ActivateAbility8()
{
	ActivateAbility(8);
}
