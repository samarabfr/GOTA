// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "TileLayoutProvider.h"
#include "TileMap.h"
#include "Algo/RandomShuffle.h"
#include "GOTA/CoreSystems/Entity/Army.h"
#include "GOTA/CoreSystems/Entity/Civilian.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingArmy.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingCivilian.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingDefense.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingSettings.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
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
	SetNetUpdateFrequency(1.0f);

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

void ATile::S_Init()
{
	AddReplicatedSubObject(EcoValues);
	Civilians.SetNumZeroed(Settings->CivilianSlots.Num());
	SpawnOceanLineMeshes();
}

void ATile::Delete()
{
	if (Army.IsValid())
	{
		Army->Delete();
	}
	for (ACivilian* Civilian : Civilians)
	{
		if (Civilian)
		{
			Civilian->Delete();
		}
	}
	Destroy();
}

TArray<ATile*> ATile::GetPathTo(ATile* Target)
{
	return ATileMap::GetPath(this, Target);
}

int32 ATile::GetPathTileDistanceTo(ATile* Target)
{
	return GetPathTo(Target).Num();
}

int32 ATile::GetTileDistanceTo(const ATile* Target) const
{
	return HexCoords.DistanceTo(Target->HexCoords);
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

void ATile::AddEntity(AEntity* Entity, const EEntityType EntityType, FVector& NewLocation)
{
	if (EntityType == EEntityType::Civilian)
	{
		ACivilian* CivilianToRemove = Cast<ACivilian>(Entity);
		AddCivilian(CivilianToRemove, NewLocation);
		return;
	}
	if (EntityType == EEntityType::Army)
	{
		AArmy* ArmyToRemove = Cast<AArmy>(Entity);
		SetArmy(ArmyToRemove, NewLocation);
		return;
	}
}

void ATile::RemoveEntity(AEntity* Entity, const EEntityType EntityType)
{
	if (EntityType == EEntityType::Civilian)
	{
		ACivilian* CivilianToRemove = Cast<ACivilian>(Entity);
		RemoveCivilian(CivilianToRemove);
		return;
	}
	if (EntityType == EEntityType::Army)
	{
		RemoveArmy();
		return;
	}
}

AArmy* ATile::GetArmy() const
{
	return Army.Get();
}

bool ATile::AcceptsArmy() const
{
	if (Terrain.Biome == EBiome::Volcano ||
		GetBuilding() &&
		GetBuilding()->GetSettings()->bDefenseEnabled)
		return false;
	return !Army.IsValid();
}

void ATile::SetArmy(AArmy* NewArmy, FVector& NewLocation)
{
	Army = NewArmy;
	if (Army.IsValid()) NewLocation = Settings->ArmySlot + GetActorLocation();
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

void ATile::RemoveCivilian(ACivilian* Civilian)
{
	for (int32 i = 0; i < Civilians.Num(); ++i)
	{
		if (Civilians[i] == Civilian) Civilians[i] = nullptr;
	}
}

// ----------------------- Building and Claiming ---------------------

void ATile::OnRep_Building(UBuilding* OldBuilding)
{
	if (OldBuilding)
	{
		OldBuilding->S_PrepareDestroy();
	}
	if (Building)
	{
		Building->C_Init();
	}
	BuildingChanged();
}

void ATile::BuildingChanged()
{
	OnBuildingChanged.Broadcast(this);
	if (Building)
	{
		SetupPopSizeChanging();
	}
}

bool ATile::CanBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder)
{
	// Check if the Building allows to be placed on this Tile
	for (FGameplayTagRule PlacementRule : BuildingDataAsset->PlacementRules)
	{
		if (!PlacementRule.IsValid(GameplayTags))
		{
			return false;
		}
	}
	// ifs instead of one condition to make debugging easier
	if (Building)
	{
		return false;
	}
	if (Terrain.Biome == EBiome::Volcano)
	{
		return false;
	}
	if (Builder == nullptr)
	{
		return false;
	}
	if (Builder->GetCountOfConstructionSites() >
		Builder->GetCountOfBuilders() + Settings->ExtraAllowedConstructionSites)
	{
		return false;
	}
	if (!Builder->IsBorderingUnclaimedTile(this))
	{
		return false;
	}
	return true;
}

bool ATile::S_TryBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder)
{
	if (!CanBuild(BuildingDataAsset, Builder))
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to build despite not being allowed."))
		return false;
	}
	return S_TryForceBuild(BuildingDataAsset, Builder);
}

bool ATile::S_TryForceBuild(UBuildingSettings* BuildingDataAsset, ASettlement* Builder)
{
	if (!Builder)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to build without a valid builder!"))
		return false;
	}
	//check if multiple production things are on
	int32 EnabledCount = 0;
	EnabledCount += BuildingDataAsset->bCivilianEnabled;
	EnabledCount += BuildingDataAsset->bArmyEnabled;
	EnabledCount += BuildingDataAsset->bDefenseEnabled;
	if (EnabledCount > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Multiple building types enabled in BuildingDataAsset. Only one allowed!"))
		return false;
	}
	// Choose fitting class
	if (BuildingDataAsset->bCivilianEnabled)
		Building = NewObject<UBuildingCivilian>();
	else if (BuildingDataAsset->bArmyEnabled)
		Building = NewObject<UBuildingArmy>();
	else if (BuildingDataAsset->bDefenseEnabled)
		Building = NewObject<UBuildingDefense>();
	else
		Building = NewObject<UBuilding>();
	Building->S_Init(BuildingDataAsset, this, Builder);
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->GetPopulation());

	GameplayTags.AppendTags(Builder->GetGameplayTags());
	GameplayTags.AppendTags(BuildingDataAsset->GameplayTags);
	GameplayTags.AddTag(Settings->BuildingUnderConstructionTag);

	MARK_PROPERTY_DIRTY_FROM_NAME(ATile, GameplayTags, this);
	MARK_PROPERTY_DIRTY_FROM_NAME(ATile, Building, this);

	Builder->S_RegisterTile(this, Building);

	OnGameplayTagsChanged.Broadcast();
	BuildingChanged();
	ValidateSpawnLayout();
	return true;
}

void ATile::S_Unbuild()
{
	if (!Building) return;
	Building->GetSettlement()->S_UnregisterTile(this, Building);
	GameplayTags.RemoveTags(Building->GetSettings()->GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	RemoveReplicatedSubObject(Building);
	RemoveReplicatedSubObject(Building->GetPopulation());
	Building->S_PrepareDestroy();
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
		if (Building) Building->S_Tick(DeltaSeconds);
	}
	else
	{
		EcoValues->ClientTick(DeltaSeconds);
		if (Building) Building->C_Tick(DeltaSeconds);
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
		if (Neighbors[i] && Neighbors[i]->Building)
			Neighbors[i]->Building->GetPopulation()->
			              NeighborChangedPopSize(Change);
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
			GameState->GetStaticMeshBatcher()->AddStaticMeshInstance(Settings->OceanLinesAtRiverDeltaMesh, T, false);
		}
		else
		{
			GameState->GetStaticMeshBatcher()->AddStaticMeshInstance(Settings->OceanLinesMesh, T, false);
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

void ATile::InitTileRotation()
{
	if (!TileContent) InitTileContent();
	const FRotator Rotator = FRotator(0, TileRotation, 0);
	TileContent->SetRotation(Rotator);
	SM_Hexagon->SetRelativeRotation(Rotator);
}

void ATile::S_InitTileRotation()
{
	int32 Rotation = 0;
	if (Terrain.bIsRiver)
	{
		Rotation = TileLayout->GetValidRotation(Terrain.RiverConnections);
	}
	else
	{
		Rotation = FMath::RandRange(0, 5);
	}
	TileRotation = Rotation * 60;
	InitTileRotation();
}

// ---------------------TileLayout--------------------

void ATile::InitTileLayout()
{
	FTileLayout* NewLayout = GetGameInstance()->GetSubsystem<UTileLayoutProvider>()->GetFittingTileLayout(Terrain);
	if (!NewLayout) return;
	TileLayout = NewLayout;
	HexagonMesh = NewLayout->HexagonMesh;
	GameplayTags.AddTag(TileLayout->LayoutTag);
	InitHexagonMesh();
	S_InitTileRotation();
	ValidateSpawnLayout();
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
	if (SpawnLayoutStruct && SpawnLayoutStruct->IsValidFor(GameplayTags)) return;
	
	SpawnLayoutStruct = FindSpawnLayoutDataAsset();
	FSpawnLayout SL;
	if (SpawnLayoutStruct)
		SL = SpawnLayoutStruct->SpawnLayout;
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
	EcoValues->SetMaxValues(SpawnLayout.Trees.Num(), SpawnLayout.Forage.Num());
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

FSpawnLayoutStruct* ATile::FindSpawnLayoutDataAsset()
{
	if (!TileLayout) return nullptr;
	
	// Find Valid Spawn Layouts
	TArray<FSpawnLayoutStruct*> PossibleLayouts;
	for (FSpawnLayoutStruct& Struct : TileLayout->SpawnLayoutStructs)
	{
		if (Struct.IsValidFor(GameplayTags))
		{
			if (Struct.GuaranteedIfPossible) return &Struct;
			PossibleLayouts.Add(&Struct);
		}
	}
	if (PossibleLayouts.Num() <= 0) return nullptr;
	// Weighted Random to select a SpawnLayout
	int32 TotalBias = 0;
	for (FSpawnLayoutStruct* Struct : PossibleLayouts)
	{
		TotalBias += Struct->SpawnBias.GetBiasAfterMultipliers(Terrain);
	}
	int Count = FMath::RandRange(0, TotalBias - 1);
	for (FSpawnLayoutStruct* Struct : PossibleLayouts)
	{
		int32 Bias = Struct->SpawnBias.GetBiasAfterMultipliers(Terrain);
		if (Count < Bias)
		{
			return Struct;
		}
		Count -= Bias;
	}
	return nullptr;
}
