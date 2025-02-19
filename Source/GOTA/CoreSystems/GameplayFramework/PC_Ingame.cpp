// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_Ingame.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputDataAsset.h"
#include "GOTA/CoreSystems/Guardian/Ability.h"
#include "GOTA/CoreSystems/Guardian/AbilityIndicator.h"
#include "GOTA/UI/Ingame/AbilitySlot.h"
#include "GOTA/CoreSystems/Guardian/AbilitySlotRegister.h"
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
	AMouseUtils* NewMouseUtils = GetWorld()->SpawnActor<AMouseUtils>(MouseUtilsClass, MouseUtilsSpawnParams);
	S_SetMouseUtils(NewMouseUtils);

	// Create AbilityIndicator
	FActorSpawnParameters AbilityIndicatorSpawnParams;
	AbilityIndicatorSpawnParams.Owner = this;
	AAbilityIndicator* NewAbilityIndicator = GetWorld()->SpawnActor<AAbilityIndicator>(
		AbilityIndicatorClass, AbilityIndicatorSpawnParams);
	S_SetAbilityIndicator(NewAbilityIndicator);
	NewMouseUtils->AttachActorToTilePosition(NewAbilityIndicator);

	// Create BuildingPlacer
	FActorSpawnParameters BuildingPlacerSpawnParams;
	BuildingPlacerSpawnParams.Owner = this;
	ABuildingPlacer* NewBuildingPlacer = GetWorld()->SpawnActor<ABuildingPlacer>(
		BuildingPlacerClass, BuildingPlacerSpawnParams);
	NewBuildingPlacer->S_Init(NewMouseUtils);
	S_SetBuildingPlacer(NewBuildingPlacer);

	// DistanceUtils
	OnGuardianChanged.AddDynamic(this, &APC_Ingame::InitDistanceUtils);

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
	OnGuardianChanged.Broadcast(Guardian);
}

void APC_Ingame::InitDistanceUtils(AGuardian* _)
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
	S_SetGuardian(Cast<AGuardian>(InPawn));

	FRotator InitialRotation = FRotator(-30.0f, 0.0f, 0.0f); // Adjust these values
	SetControlRotation(InitialRotation);
}

void APC_Ingame::S_SetGuardian(AGuardian* NewGuardian)
{
	Guardian = NewGuardian;
	OnGuardianChanged.Broadcast(Guardian);
	MARK_PROPERTY_DIRTY_FROM_NAME(APC_Ingame, Guardian, this)
}

// ---------------------- InteractionMode ----------------------

void APC_Ingame::ClickActor()
{
	if (!IngameUI) return;
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

// ---------------------------------------- Ability ----------------------------------------


void APC_Ingame::ActivateCurrentlyTargetingAbility()
{
	if (!CurrentlyTargeting.IsValid()) return;

	CurrentlyTargeting.Get()->ActivateAbility(MouseUtils->GetHoverAbilityTarget());
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

void APC_Ingame::ActivateAbility(FName SlotName)
{
	UAbilitySlotRegister* AbilityManager = GetGameInstance()->GetSubsystem<UAbilitySlotRegister>();
	if (AbilityManager)
	{
		if (UAbilitySlot* AbilitySlot = AbilityManager->GetAbilitySlot(SlotName))
		{
			ActivateAbility(AbilitySlot);
		}
	}
}

void APC_Ingame::ActivateAbility(UAbilitySlot* Slot)
{
	if (!Slot) return;

	AAbility* Ability = Slot->GetAbility();

	// Either Input was invalid or there is no skill in the selected slot, either way we tried to activate
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

void APC_Ingame::LearnAbility(UAbilitySettings* AbilitySettings)
{
	if (!Guardian) return;

	const UAbilitySlotRegister* AbilityManager = GetGameInstance()->GetSubsystem<UAbilitySlotRegister>();
	const FName AbilitySlotName = AbilityManager->GetFreeAbilitySlotName();
	if (!AbilitySlotName.IsNone())
	{
		Guardian->LearnAbility(AbilitySettings, AbilitySlotName);
	}
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
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->AddMappingContext(InputDataAsset->MappingContext, 1);

	UEnhancedInputComponent* Component = Cast<UEnhancedInputComponent>(InputComponent);

	Component->BindAction(InputDataAsset->LeftClick, ETriggerEvent::Triggered, this, &APC_Ingame::LeftClick);

	Component->BindAction(InputDataAsset->Jump, ETriggerEvent::Triggered, this, &APC_Ingame::StartJump);
	Component->BindAction(InputDataAsset->Jump, ETriggerEvent::Canceled, this, &APC_Ingame::StopJump);
	Component->BindAction(InputDataAsset->Jump, ETriggerEvent::Completed, this, &APC_Ingame::StopJump);

	Component->BindAction(InputDataAsset->LookAround, ETriggerEvent::Triggered, this, &APC_Ingame::LookAround);
	Component->BindAction(InputDataAsset->ActivateLooking, ETriggerEvent::Started, this,
	                      &APC_Ingame::StartLookingAround);
	Component->BindAction(InputDataAsset->ActivateLooking, ETriggerEvent::Completed, this,
	                      &APC_Ingame::StopLookingAround);

	Component->BindAction(InputDataAsset->Escape, ETriggerEvent::Triggered, this, &APC_Ingame::HandleEscapePressed);
	Component->BindAction(InputDataAsset->BuildMenu, ETriggerEvent::Triggered, this, &APC_Ingame::ToggleBuildMenu);
	Component->BindAction(InputDataAsset->DebugMenu, ETriggerEvent::Triggered, this, &APC_Ingame::ToggleDebugMenu);

	Component->BindAction(InputDataAsset->Ability1, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility1);
	Component->BindAction(InputDataAsset->Ability2, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility2);
	Component->BindAction(InputDataAsset->Ability3, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility3);
	Component->BindAction(InputDataAsset->Ability4, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility4);
	Component->BindAction(InputDataAsset->Ability5, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility5);
	Component->BindAction(InputDataAsset->Ability6, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility6);
	Component->BindAction(InputDataAsset->Ability7, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility7);
	Component->BindAction(InputDataAsset->Ability8, ETriggerEvent::Triggered, this, &APC_Ingame::ActivateAbility8);
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

void APC_Ingame::HandleEscapePressed()
{
	if (!IngameUI) return;
	IngameUI->HandleEscapePressed();
}

void APC_Ingame::ToggleBuildMenu()
{
	if (!IngameUI) return;
	IngameUI->ToggleBuildMenu();
}

void APC_Ingame::ToggleDebugMenu()
{
	if (!IngameUI) return;
	IngameUI->ToggleDebugMenu();
}

void APC_Ingame::ActivateAbility1()
{
	ActivateAbility(FName("AbilityBar1"));
}

void APC_Ingame::ActivateAbility2()
{
	ActivateAbility(FName("AbilityBar2"));
}

void APC_Ingame::ActivateAbility3()
{
	ActivateAbility(FName("AbilityBar3"));
}

void APC_Ingame::ActivateAbility4()
{
	ActivateAbility(FName("AbilityBar4"));
}

void APC_Ingame::ActivateAbility5()
{
	ActivateAbility(FName("AbilityBar5"));
}

void APC_Ingame::ActivateAbility6()
{
	ActivateAbility(FName("AbilityBar6"));
}

void APC_Ingame::ActivateAbility7()
{
	ActivateAbility(FName("AbilityBar7"));
}

void APC_Ingame::ActivateAbility8()
{
	ActivateAbility(FName("AbilityBar8"));
}
