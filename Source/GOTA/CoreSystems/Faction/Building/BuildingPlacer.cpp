// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPlacer.h"

#include "BuildingPlacerSettings.h"
#include "BuildingSettings.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
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

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
}

bool ABuildingPlacer::IsSupportedForNetworking() const
{
	return true;
}

ABuildingPlacer::ABuildingPlacer()
{
	static ConstructorHelpers::FObjectFinder<UBuildingPlacerSettings> SettingsFinder(
		TEXT("/Game/CoreSystems/Faction/DA_BuildingPlacer"));
	if (SettingsFinder.Succeeded())
		Settings = SettingsFinder.Object;

	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.5f;

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
	const AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();
}

void ABuildingPlacer::Init(AMouseUtils* InMouseUtils)
{
	MouseUtils = InMouseUtils;
	MouseUtils->AttachToTilePosition(this);
	MouseUtils->OnHoverTileChanged.AddDynamic(this, &ABuildingPlacer::RefreshPlaceability);
}

void ABuildingPlacer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (MouseUtils)
		RefreshPlaceability(MouseUtils->GetHoverTile());
}

void ABuildingPlacer::StartPlacingBuilding(UBuildingSettings* Building)
{
	bIsPlacingBuilding = true;
	BuildingToPlace = Building;
	MeshComponent->SetVisibility(true);
	//Show it somehow
}

void ABuildingPlacer::StopPlacingBuilding()
{
	bIsPlacingBuilding = false;
	MeshComponent->SetVisibility(false);
}

void ABuildingPlacer::PlaceBuilding()
{
	if (MouseUtils && MouseUtils->GetHoverTile() && BuildingToPlace)
	{
		const AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
		MouseUtils->GetHoverTile()->TryBuild(BuildingToPlace, GameState->GetTribe());
		StopPlacingBuilding();
	}
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
		if (Neighbor && Neighbor->GetClaimant() && Neighbor->GetClaimant()->Affiliation == EAffiliation::Ally)
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
