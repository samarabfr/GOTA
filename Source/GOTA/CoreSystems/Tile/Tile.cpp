// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Algo/RandomShuffle.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "GOTA/CoreSystems/Utility/StaticMeshBatcher.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// ---------------------------------------------------------
// Initialisation and core variables

void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ATile, HexCoords, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, Neighbors, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, Terrain, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, TileRotation, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, HexagonMesh, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ATile, SpawnLayout, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, EcoValues, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, GameplayTags, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, Building, Params);
}

ATile::ATile()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5f;

	Neighbors.SetNumZeroed(6);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SM_Hexagon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Hexagon"));
	SM_Hexagon->SetupAttachment(RootComponent);

	EcoValues = CreateDefaultSubobject<UEcoValues>(TEXT("EcoValues"));
	SetupEcoValuesChanging();
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameState<AGS_Ingame>()->IncrementReplicationCount();
	if (!HasAuthority())
	{
		SpawnOceanLineMeshes();
	}
}

void ATile::ServerInit()
{
	AddReplicatedSubObject(EcoValues);
	Civilians.SetNumZeroed(Settings->CivilianSlots.Num());
	SpawnOceanLineMeshes();
}

void ATile::OnRep_GameplayTags()
{
	OnGameplayTagsChanged.Broadcast();
}

bool ATile::AcceptsEntity(const EEntityType EntityType) const
{
	if (EntityType == EEntityType::Civilian)
		return AcceptsCivilian();
	if (EntityType == EEntityType::Army)
		return AcceptsArmy();
	return false;
}

AArmy* ATile::GetArmy() const
{
	return Army;
}

bool ATile::AcceptsArmy() const
{
	if (Terrain.Biome == EBiome::Volcano)
		return false;
	return !Army;
}

void ATile::SetArmy(AArmy* NewArmy, FVector& NewLocation)
{
	Army = NewArmy;
	if (Army) NewLocation = Settings->ArmySlot + GetActorLocation();
}

void ATile::RemoveArmy()
{
	FVector _;
	SetArmy(nullptr, _);
}

bool ATile::AcceptsCivilian() const
{
	if (Terrain.Biome == EBiome::Volcano)
		return false;
	for (const ACivilian* Civilian : Civilians)
	{
		if (!Civilian) return true;
	}
	return false;
}

void ATile::AddCivilian(ACivilian* Civilian, FVector& NewLocation)
{
	for (int32 i = 0; i < Civilians.Num(); ++i)
	{
		if (!Civilians[i])
		{
			Civilians[i] = Civilian;
			NewLocation = Settings->CivilianSlots[i] + GetActorLocation();
			return;
		}
	}
}

void ATile::RemoveCivilian(const ACivilian* Civilian)
{
	for (int32 i = 0; i < Civilians.Num(); ++i)
	{
		if (Civilians[i] == Civilian) Civilians[i] = nullptr;
	}
}

// ----------------------- Building and Claiming ---------------------

void ATile::OnRep_Building()
{
	if (Building)
	{
		Building->ClientInit();
	}
	BuildingChanged();
}

void ATile::BuildingChanged()
{
	OnBuildingChanged.Broadcast(this);
	if (Building)
	{
		SetupPopSizeChanging();
		UpdateClaimWallsWithNeighbors();
	}
}

bool ATile::CanBuild()
{
	return !Building && Terrain.Biome != EBiome::Volcano;
}

bool ATile::TryBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder)
{
	if (!CanBuild() || !Builder) return false;
	Building = NewObject<UBuilding>();
	Building->ServerInit(BuildingDataAsset, this, Builder);
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->GetPopulation());

	UpdateClaimWallsWithNeighbors();

	GameplayTags.AppendTags(Builder->GetGameplayTags());
	GameplayTags.AppendTags(BuildingDataAsset->GameplayTags);
	GameplayTags.AddTag(Settings->BuildingUnderConstructionTag);

	MARK_PROPERTY_DIRTY_FROM_NAME(ATile, GameplayTags, this);
	MARK_PROPERTY_DIRTY_FROM_NAME(ATile, Building, this);

	OnGameplayTagsChanged.Broadcast();
	BuildingChanged();
	ValidateSpawnLayout();
	return true;
}

void ATile::Unbuild()
{
	if (!Building) return;
	GameplayTags.RemoveTags(Building->Settings->GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	RemoveReplicatedSubObject(Building);
	RemoveReplicatedSubObject(Building->GetPopulation());
	Building = nullptr;
	BuildingChanged();
	ValidateSpawnLayout();
}

void ATile::OnBuildingFinishedConstruction()
{
	GameplayTags.RemoveTag(Settings->BuildingUnderConstructionTag);
	MARK_PROPERTY_DIRTY_FROM_NAME(ATile, GameplayTags, this);
	OnGameplayTagsChanged.Broadcast();
}

ASettlement* ATile::GetClaimant() const
{
	if (Building)
	{
		return Building->Settlement;
	}
	return nullptr;
}

void ATile::UpdateClaimWallsWithNeighbors()
{
	for (ATile* Neighbor : Neighbors)
	{
		if (Neighbor) Neighbor->UpdateClaimWalls();
	}
	UpdateClaimWalls();
}

void ATile::UpdateClaimWalls()
{
	if (IsClaimed())
	{
		for (uint8 i = 0; i < 6; ++i)
		{
			if (!Neighbors[i] || !Neighbors[i]->IsClaimed() || Neighbors[i]->GetClaimant() != GetClaimant())
			{
				// Should have flag in this direction
				if (!ClaimWallsInstanceIds.Contains(i))
				{
					// doesn't have one yet, so we make one
					FTransform Transform = FTransform();
					Transform.SetLocation(GetActorLocation());
					Transform.SetRotation(
						FRotator(0, 60 * i, 0).Quaternion());
					ClaimWallsInstanceIds.Add(i, GameState->GetStaticMeshBatcher()->AddStaticMeshInstance(
						                          Settings->ClaimMesh, Transform));
				}
			}
			else if (ClaimWallsInstanceIds.Contains(i))
			{
				// Should NOT have flag in this direction
				GameState->GetStaticMeshBatcher()->RemoveStaticMeshInstance(
					Settings->ClaimMesh, *ClaimWallsInstanceIds.Find(i));
				ClaimWallsInstanceIds.Remove(i);
			}
		}
	}
	else
	{
		// remove all flags
		for (TTuple<uint8, FPrimitiveInstanceId> Tuple : ClaimWallsInstanceIds)
		{
			GameState->GetStaticMeshBatcher()->RemoveStaticMeshInstance(Settings->ClaimMesh, Tuple.Value);
		}
		ClaimWallsInstanceIds.Empty();
	}
}

// ------------------- Ticking -------------------------

void ATile::GOTATick()
{
	if (LastTick < 0)
	{
		LastTick = GetWorld()->GetTimeSeconds();
		return;
	}
	double DeltaSeconds = GetWorld()->GetTimeSeconds() - LastTick;
	if (HasAuthority())
	{
		EcoValues->ServerTick(DeltaSeconds);
		if (Building) Building->ServerTick(DeltaSeconds);
	}
	else
	{
		EcoValues->ClientTick(DeltaSeconds);
		if (Building) Building->ClientTick(DeltaSeconds);
	}
	LastTick = GetWorld()->GetTimeSeconds();
}

void ATile::SetupPopSizeChanging()
{
	Building->GetPopulation()->OnSizeChanged.AddDynamic(this, &ATile::PopSizeChanged);
	// notify the neighbors of this population size
	PopSizeChanged(Building->GetPopulation()->GetSize());
	// notify this population of all neighbor population sizes
	for (ATile* Neighbor : Neighbors)
	{
		if (Neighbor && Neighbor->Building)
			Building->GetPopulation()->NeighborChangedPopSize(Neighbor->Building->GetPopulation()->GetSize());
	}
}

void ATile::PopSizeChanged(const int16 Change)
{
	ForceNetUpdate();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i] && Neighbors[i]->Building) Neighbors[i]->Building->GetPopulation()->NeighborChangedPopSize(Change);
	}
}

void ATile::SetupEcoValuesChanging()
{
	EcoValues->OnTreesChanged.AddDynamic(this, &ATile::TreesChanged);
	EcoValues->OnForageChanged.AddDynamic(this, &ATile::ForageChanged);
}

void ATile::TreesChanged(const int32 Change)
{
	ForceNetUpdate();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->EcoValues->NeighborChangedTrees(Change);
	}
}

void ATile::ForageChanged(const int32 Change)
{
	ForceNetUpdate();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->EcoValues->NeighborChangedForage(Change);
	}
}

// -----------------------Graphics--------------------------

void ATile::InitHexagonMesh()
{
	SM_Hexagon->SetStaticMesh(HexagonMesh);
	if (HexagonMesh) UpdateHexagonMaterial();
}

// ----------------------- Terrain ---------------------------

void ATile::C_TerrainInit()
{
	if (!TileContent) InitTileContent();
	TileContent->SetTerrain(Terrain);
	UpdateHexagonMaterial();
	EcoValues->Init(Terrain.Biome);
}

void ATile::S_TerrainInit(const FTerrain& Terrain_)
{
	FTerrain OldTerrain = Terrain;
	Terrain = Terrain_;
	GameplayTags.AddTag(DA_Biomes->EnumToTag[Terrain_.Biome]);
	C_TerrainInit();
	InitTileLayout();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ATile::UpdateHexagonMaterial()
{
	switch (Terrain.Biome)
	{
	case EBiome::Gras:
		SM_Hexagon->SetMaterial(0, Settings->M_Grass);
		break;
	case EBiome::Beach:
		SM_Hexagon->SetMaterial(0, Settings->M_Beach);
		break;
	case EBiome::Mountain:
		SM_Hexagon->SetMaterial(0, Settings->M_Mountain);
		break;
	case EBiome::Volcano:
		SM_Hexagon->SetMaterial(0, Settings->M_Volcano);
		break;
	}
}

void ATile::SpawnOceanLineMeshes()
{
	if (!GameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("OceanLines couldn't be spawned, because GameState is nullptr"))
		return;
	}

	for (int32 i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) continue;

		FTransform T = FTransform();

		FVector Location = GetActorLocation();
		Location.Z = 1.0f;
		T.SetLocation(Location);
		T.SetRotation(FRotator(0, i * 60 + 180, 0).Quaternion());

		if (Terrain.RiverConnections[i])
		{
			GameState->GetStaticMeshBatcher()->AddStaticMeshInstance(Settings->OceanLinesAtRiverDeltaMesh, T);
		}
		else
		{
			GameState->GetStaticMeshBatcher()->AddStaticMeshInstance(Settings->OceanLinesMesh, T);
		}
	}
}

// ------------------TileContent--------------------

void ATile::InitTileContent()
{
	TileContent = NewObject<UTileContent>();
	if (!GameState) GameState = GetWorld()->GetGameState<AGS_Ingame>();
	TileContent->Init(this, GameState);
}

void ATile::ClientInitTileRotation()
{
	if (!TileContent) InitTileContent();
	const FRotator Rotator = FRotator(0, TileRotation, 0);
	TileContent->SetRotation(Rotator);
	SM_Hexagon->SetRelativeRotation(Rotator);
}

void ATile::ServerInitTileRotation()
{
	int32 Rotation = 0;
	if (Terrain.bIsRiver)
	{
		Rotation = FindAValidRiverConnectionRotation(TileLayout->RiverConnections);
	}
	else
	{
		Rotation = FMath::RandRange(0, 5);
	}
	TileRotation = Rotation * -60;
	ClientInitTileRotation();
}

// ---------------------TileLayout--------------------

void ATile::InitTileLayout()
{
	FTileLayout* NewLayout = FindTileLayout();
	if (!NewLayout) return;
	TileLayout = NewLayout;
	HexagonMesh = NewLayout->HexagonMesh;
	InitHexagonMesh();
	ServerInitTileRotation();
	ValidateSpawnLayout();
}

FTileLayout* ATile::FindTileLayout()
{
	FString _;
	TArray<FTileLayout*> AllRows;
	Settings->TileLayouts->GetAllRows<FTileLayout>(_, AllRows);

	for (FTileLayout* Row : AllRows)
	{
		if (IsValidTileLayout(Row))
		{
			return Row;
		}
	}
	return nullptr;
}

bool ATile::IsValidTileLayout(const FTileLayout* Layout) const
{
	// Tile has River but Row doesn't allow that
	if (Terrain.bIsRiver != Layout->HasRiver) return false;
	// Is a river but can't find a working Rotation
	if (Terrain.bIsRiver && FindAValidRiverConnectionRotation(Layout->RiverConnections) < 0)
		return false;
	return true;
}

int32 ATile::FindAValidRiverConnectionRotation(const TArray<bool> Connections) const
{
	if (Connections.Num() != 6) return -1;
	for (int32 Rotation = 0; Rotation < 6; ++Rotation)
	{
		bool ThisRotationWorks = true;
		for (int i = 0; i < 6; ++i)
		{
			if (Terrain.RiverConnections[i] != Connections[(i + Rotation) % 6])
			{
				ThisRotationWorks = false;
				break;
			}
		}
		if (ThisRotationWorks) return Rotation;
	}
	return -1;
}

// ---------------------SpawnLayout--------------------

void ATile::SetSpawnLayout(const FSpawnLayout& SpawnLayout_)
{
	if (!TileContent) InitTileContent();
	SpawnLayout = SpawnLayout_;
	MARK_PROPERTY_DIRTY_FROM_NAME(ATile, SpawnLayout, this)
	TileContent->OnSpawnPointLayoutChanged();
}

void ATile::OnRep_SpawnPointLayout()
{
	if (!TileContent) InitTileContent();
	TileContent->OnSpawnPointLayoutChanged();
}

void ATile::ValidateSpawnLayout()
{
	// check if current SpawnPointLayout still works
	if (SpawnLayoutDataAsset && SpawnLayoutDataAsset->IsValidFor(GameplayTags)) return;
	SpawnLayoutDataAsset = FindSpawnLayoutDataAsset();
	FSpawnLayout SL;
	if (SpawnLayoutDataAsset)
		SL = SpawnLayoutDataAsset->SpawnLayout;
	else
		SL = FSpawnLayout();

	// Apply Spawn Chances and shuffle all arrays. The shuffling causes every Tile with the same SpawnLayout
	// to have a different order in which they spawn the individual trees

	ApplySpawnChances(SL.Trees);
	Algo::RandomShuffle(SL.Trees);

	ApplySpawnChances(SL.Forage);
	Algo::RandomShuffle(SL.Forage);

	ApplySpawnChances(SL.Props);
	Algo::RandomShuffle(SL.Props);

	ApplySpawnChances(SL.Buildings);
	Algo::RandomShuffle(SL.Buildings);

	SetSpawnLayout(SL);
	EcoValues->SetMaxValues(SpawnLayout.Trees.Num(), Terrain.Biome);
}

void ATile::ApplySpawnChances(TArray<FSpawnPoint>& SpawnPoints)
{
	// Check for Spawn points to remove
	for (int i = SpawnPoints.Num() - 1; i >= 0; --i)
	{
		if (SpawnPoints[i].SpawnChance != 100 && SpawnPoints[i].SpawnChance <= FMath::RandRange(0, 99))
		{
			SpawnPoints.RemoveAt(i);
		}
	}
}

USpawnLayoutDataAsset* ATile::FindSpawnLayoutDataAsset()
{
	// Find Valid Spawn Layouts
	TArray<USpawnLayoutDataAsset*> PossibleLayouts;
	for (USpawnLayoutDataAsset* DA_SpawnLayout : TileLayout->SpawnLayouts)
	{
		if (DA_SpawnLayout && DA_SpawnLayout->IsValidFor(GameplayTags))
		{
			if (DA_SpawnLayout->GuaranteedIfPossible) return DA_SpawnLayout;
			PossibleLayouts.Add(DA_SpawnLayout);
		}
	}
	if (PossibleLayouts.Num() <= 0) return nullptr;
	// Weighted Random to select a SpawnLayout
	int32 TotalBias = 0;
	for (USpawnLayoutDataAsset* DA_SpawnLayout : PossibleLayouts)
	{
		TotalBias += DA_SpawnLayout->SpawnBias.GetBiasAfterMultipliers(Terrain);
	}
	int Count = FMath::RandRange(0, TotalBias - 1);
	for (USpawnLayoutDataAsset* DA_SpawnLayout : PossibleLayouts)
	{
		int32 Bias = DA_SpawnLayout->SpawnBias.GetBiasAfterMultipliers(Terrain);
		if (Count < Bias)
		{
			return DA_SpawnLayout;
		}
		Count -= Bias;
	}
	return nullptr;
}
