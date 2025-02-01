// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPlacer.h"

#include "BuildingPlacerSettings.h"
#include "BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Utility/MouseUtils.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void ABuildingPlacer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ABuildingPlacer, MouseUtils, Params)

	Params.Condition = COND_SkipOwner;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ABuildingPlacer, BuildingToPlace, Params)
}

bool ABuildingPlacer::IsSupportedForNetworking() const
{
	return true;
}

// ----------------- LifeCycle -----------------

ABuildingPlacer::ABuildingPlacer()
{
	static ConstructorHelpers::FObjectFinder<UBuildingPlacerSettings> SettingsFinder(
		TEXT("/Game/CoreSystems/Faction/DA_BuildingPlacer"));
	if (SettingsFinder.Succeeded())
		Settings = SettingsFinder.Object;

	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	SetNetUpdateFrequency(1.0f);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 2.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>("ROOT");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetVisibility(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Settings)
	{
		MeshComponent->SetStaticMesh(Settings->Mesh);
		MeshComponent->SetMaterial(0, Settings->PlacingPossibleMaterial);
	}
}

void ABuildingPlacer::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->IncrementReplicationCount();
}

void ABuildingPlacer::S_Init(AMouseUtils* InMouseUtils)
{
	MouseUtils = InMouseUtils;
	MouseUtils->AttachActorToTilePosition(this);
	MouseUtils->OnHoverTileChanged.AddDynamic(this, &ABuildingPlacer::RefreshPlaceability);
}

void ABuildingPlacer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (MouseUtils)
		RefreshPlaceability(MouseUtils->GetHoverTile());
}

// ------------- Variables -----------------

void ABuildingPlacer::OnRep_MouseUtils()
{
	if (MouseUtils)
	{
		MouseUtils->OnHoverTileChanged.AddDynamic(this, &ABuildingPlacer::RefreshPlaceability);
	}
}

// ----------------- Placing -----------------

void ABuildingPlacer::PlaceBuilding()
{
	ATile* HoverTile = nullptr;
	if (MouseUtils)
	{
		HoverTile = MouseUtils->GetHoverTile();
	}
	if (HoverTile && BuildingToPlace && CanPlace(HoverTile))
	{
		SRPC_PlaceBuilding(HoverTile, BuildingToPlace);
	}
	StopPlacingBuilding();
}

void ABuildingPlacer::SRPC_PlaceBuilding_Implementation(ATile* Tile, UBuildingSettings* Building)
{
	if (!Tile || !Building || !CanPlace(Tile)) return;

	const AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	Tile->S_TryBuild(BuildingToPlace, GameState->GetTribe());
}

void ABuildingPlacer::RefreshPlaceability(ATile* NewTile)
{
	if (CanPlace(MouseUtils->GetHoverTile()))
	{
		MeshComponent->SetMaterial(0, Settings->PlacingPossibleMaterial);
	}
	else
	{
		MeshComponent->SetMaterial(0, Settings->PlacingImpossibleMaterial);
	}
}

bool ABuildingPlacer::CanPlace(ATile* Tile)
{
	if (!Tile || !BuildingToPlace) return false;
	// Check if Tile is next to the Tribe
	bool bNextToTribe = false;
	for (ATile* Neighbor : Tile->Neighbors)
	{
		if (Neighbor && Neighbor->GetClaimant() && Neighbor->GetClaimant()->GetAffiliation() == EAffiliation::Ally)
		{
			bNextToTribe = true;
			break;
		}
	}
	if (!bNextToTribe) return false;
	// Check if the Building allows to be placed on this Tile
	for (FGameplayTagRule PlacementRule : BuildingToPlace->PlacementRules)
	{
		if (!PlacementRule.IsValid(Tile->GameplayTags))
			return false;
	}
	return true;
}

// ----------------- Start & Stop Placing -----------------

void ABuildingPlacer::StartPlacingBuilding(UBuildingSettings* Building)
{
	SRPC_StartPlacingBuilding(Building);
	if (!HasAuthority())
	{
		// if the server calls this, it does this in the RPC
		BuildingToPlace = Building;
		StartShowingPlacingBuilding();
	}
}

void ABuildingPlacer::StopPlacingBuilding()
{
	SRPC_StopPlacingBuilding();
	if (!HasAuthority())
	{
		// if the server calls this, it does this in the RPC
		BuildingToPlace = nullptr;
		StopShowingPlacingBuilding();
	}
}

void ABuildingPlacer::SRPC_StartPlacingBuilding_Implementation(UBuildingSettings* Building)
{
	BuildingToPlace = Building;
	MARK_PROPERTY_DIRTY_FROM_NAME(ABuildingPlacer, BuildingToPlace, this)
	ForceNetUpdate();
	StartShowingPlacingBuilding();
}

void ABuildingPlacer::SRPC_StopPlacingBuilding_Implementation()
{
	BuildingToPlace = nullptr;
	MARK_PROPERTY_DIRTY_FROM_NAME(ABuildingPlacer, BuildingToPlace, this)
	ForceNetUpdate();
	StopShowingPlacingBuilding();
}

void ABuildingPlacer::OnRep_BuildingToPlace()
{
	if (BuildingToPlace)
	{
		StartShowingPlacingBuilding();
	}
	else
	{
		StopShowingPlacingBuilding();
	}
}

void ABuildingPlacer::StartShowingPlacingBuilding()
{
	MeshComponent->SetVisibility(true);
}

void ABuildingPlacer::StopShowingPlacingBuilding()
{
	MeshComponent->SetVisibility(false);
}
