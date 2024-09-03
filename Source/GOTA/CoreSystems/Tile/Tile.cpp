// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

bool ATile::bFreezeGrowthChanges = false;

// ---------------------------------------------------------
// Initialisation and core variables

void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATile, HexCoords);
	DOREPLIFETIME(ATile, Neighbors);
	DOREPLIFETIME(ATile, GameplayTags);

	DOREPLIFETIME(ATile, Trees);
	DOREPLIFETIME(ATile, TreeGrowth);
	DOREPLIFETIME(ATile, TreeGrowthChange);
	DOREPLIFETIME(ATile, Forage);
	DOREPLIFETIME(ATile, ForageChange);
	DOREPLIFETIME(ATile, Wildlife);
	DOREPLIFETIME(ATile, WildlifeGrowth);
	DOREPLIFETIME(ATile, WildlifeGrowthChange);

	DOREPLIFETIME(ATile, Building);
	DOREPLIFETIME(ATile, Claimant);
	DOREPLIFETIME(ATile, AlliedEntity);
	DOREPLIFETIME(ATile, EnemyEntity);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	Params.Condition = COND_InitialOnly;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ATile, Terrain, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, TileRotation, Params);
	DOREPLIFETIME_WITH_PARAMS(ATile, HexagonMesh, Params);

	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS(ATile, SpawnLayout, Params);
}

ATile::ATile()
{
	// Replication Setup
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	// Tick Setup
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5f;

	// initialize neighbor array
	Neighbors.SetNumZeroed(6);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SM_Hexagon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Hexagon"));
	SM_Hexagon->SetupAttachment(RootComponent);

	// Subobjects
	Trees = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Trees"));
	TreeGrowth = CreateDefaultSubobject<UGOTAAttribute>(TEXT("TreeGrowth"));
	TreeGrowthChange = CreateDefaultSubobject<UGOTAAttribute>(TEXT("TreeGrowthChange"));
	Forage = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Forage"));
	ForageChange = CreateDefaultSubobject<UGOTAAttribute>(TEXT("ForageChange"));
	Wildlife = CreateDefaultSubobject<UGOTAAttributeLimited>(TEXT("Wildlife"));
	WildlifeGrowth = CreateDefaultSubobject<UGOTAAttribute>(TEXT("WildlifeGrowth"));
	WildlifeGrowthChange = CreateDefaultSubobject<UGOTAAttribute>(TEXT("WildlifeGrowthChange"));
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();
}

void ATile::ServerInit()
{
	AddReplicatedSubObject(Trees);
	AddReplicatedSubObject(TreeGrowth);
	AddReplicatedSubObject(TreeGrowthChange);
	AddReplicatedSubObject(Forage);
	AddReplicatedSubObject(ForageChange);
	AddReplicatedSubObject(Wildlife);
	AddReplicatedSubObject(WildlifeGrowth);
	AddReplicatedSubObject(WildlifeGrowthChange);

	Trees->OnChanged.AddDynamic(this, &ATile::CalculateTreeGrowthChangeWithNeighbors);
	Forage->OnChanged.AddDynamic(this, &ATile::CalculateForageChangeWithNeighbors);
	Forage->SetMaximum(BalanceData->MaxForage);
	Forage->SetCurrent(BalanceData->StartingForage);
	Wildlife->OnChanged.AddDynamic(this, &ATile::CalculateWildlifeGrowthChangeWithNeighbors);
	Wildlife->SetMaximum(BalanceData->MaxWildlife);
	Wildlife->SetCurrent(BalanceData->StartingWildlife);
	Trees->SetCurrent(BalanceData->StartingTrees);
}

void ATile::OnRep_GameplayTags()
{
	OnGameplayTagsChanged.Broadcast();
}

AEntity* ATile::GetAlliedEntity()
{
	return AlliedEntity;
}

AEntity* ATile::GetEnemyEntity()
{
	return EnemyEntity;
}

void ATile::SetAlliedEntity(AEntity* NewAlliedEntity)
{
	AEntity* OldEntity = AlliedEntity;
	AlliedEntity = NewAlliedEntity;
	OnEntityChanged.Broadcast(this, OldEntity);
}

void ATile::SetEnemyEntity(AEntity* NewEnemyEntity)
{
	AEntity* OldEntity = EnemyEntity;
	EnemyEntity = NewEnemyEntity;
	OnEntityChanged.Broadcast(this, OldEntity);
}

AEntity* ATile::GetEntity(EAffiliation Affiliation)
{
	if (Affiliation == EAffiliation::Ally)
		return GetAlliedEntity();
	return GetEnemyEntity();
}

void ATile::SetEntity(AEntity* NewEntity, EAffiliation Affiliation)
{
	if (Affiliation == EAffiliation::Ally)
		SetAlliedEntity(NewEntity);
	else
		SetEnemyEntity(NewEntity);
}

bool ATile::IsWalkable(EAffiliation Affiliation) const
{
	if (Affiliation == EAffiliation::Ally)
	{
		return !AlliedEntity;
	}
	if (Affiliation == EAffiliation::Enemy)
	{
		return !EnemyEntity;
	}
	return false;
}

AEntity* ATile::GetEntityByAffiliation(EAffiliation Affiliation) const
{
	if (Affiliation == EAffiliation::Ally) return AlliedEntity;
	if (Affiliation == EAffiliation::Enemy) return EnemyEntity;
	return nullptr;
}

// ---------------------------------------------------------
// Claimant and claiming

ASettlement* ATile::GetClaimant()
{
	return Claimant;
}

void ATile::SetClaimant(ASettlement* NewClaimant)
{
	Claimant = NewClaimant;
	UpdateClaimWallsWithNeighbors();
}

void ATile::OnRep_Claimant(ASettlement* NewClaimant)
{
	UpdateClaimWallsWithNeighbors();
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
	if (Claimant)
	{
		for (uint8 i = 0; i < 6; ++i)
		{
			if (!Neighbors[i] || !Neighbors[i]->Claimant || Neighbors[i]->Claimant != Claimant)
			{
				// Should have flag in this direction
				if (!ClaimWallsInstanceIds.Contains(i))
				{
					// doesn't have one yet, so we make one
					FTransform Transform = FTransform();
					Transform.SetLocation(GetActorLocation());
					Transform.SetRotation(
						FRotator(0, 60 * i, 0).Quaternion());
					ClaimWallsInstanceIds.Add(i, Claimant->AddClaimMeshInstance(Transform));
				}
			}
			else if (ClaimWallsInstanceIds.Contains(i))
			{
				// Should NOT have flag in this direction
				Claimant->RemoveClaimMeshInstance(*ClaimWallsInstanceIds.Find(i));
				ClaimWallsInstanceIds.Remove(i);
			}
		}
	}
	else
	{
		// remove all flags
		for (TTuple<uint8, FPrimitiveInstanceId> Tuple : ClaimWallsInstanceIds)
		{
			Claimant->RemoveClaimMeshInstance(Tuple.Value);
		}
		ClaimWallsInstanceIds.Empty();
	}
}

bool ATile::IsClaimable() const
{
	return !Claimant;
}

bool ATile::TryClaim(ASettlement* PotentialClaimant)
{
	if (!IsClaimable()) return false;
	Claimant = PotentialClaimant;
	if (Building)
	{
		Claimant->OnBuildingAdded(Building);
	}
	GameplayTags.AppendTags(Claimant->GameplayTags);
	UpdateClaimWallsWithNeighbors();
	OnGameplayTagsChanged.Broadcast();
	return true;
}

void ATile::Unclaim()
{
	if (!Claimant) return;
	if (Building)
	{
		Claimant->OnBuildingRemoved(Building);
	}
	Claimant->LostClaim(this);
	GameplayTags.RemoveTags(Claimant->GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	Claimant = nullptr;
}

// ---------------------------------------------------------
// Building

void ATile::OnRep_Building()
{
	OnBuildingChanged.Broadcast(this);
}

bool ATile::CanBuild()
{
	return !Building;
}

bool ATile::TryBuild(UBuildingDataAsset* BuildingDataAsset)
{
	// There is already a Building, can't build here
	if (Building) return false;
	// Create Building Object
	Building = NewObject<UBuilding>();
	Building->DataAsset = BuildingDataAsset;
	if (Claimant)
	{
		Claimant->OnBuildingAdded(Building);
	}
	Building->PopContainer->ChangeMaxSize(BuildingDataAsset->TierOne.Housing);
	Building->SetupProduction(&BuildingDataAsset->TierOne);
	// Add Building related GameplayTags
	GameplayTags.AppendTags(BuildingDataAsset->TierOne.GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	// Replication stuff
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->PopContainer);
	// Population stuff
	Building->PopContainer->OnPopulationChanged.AddDynamic(this, &ATile::CalculatePopulationGrowthChangeWithNeighbors);
	CalculatePopulationGrowthChangeWithNeighbors(FPopulation());
	// Set Graphics
	OnBuildingChanged.Broadcast(this);
	InitTileLayout();
	return true;
}

bool ATile::CanUpgrade()
{
	return Building->CanUpgrade();
}

bool ATile::TryUpgrade()
{
	if (!Building || !Building->CanUpgrade()) return false;
	GameplayTags.RemoveTags(Building->DataAsset->GetTierData(Building->Tier)->GameplayTags);
	Building->Upgrade();
	GameplayTags.AppendTags(Building->DataAsset->GetTierData(Building->Tier)->GameplayTags);
	OnBuildingChanged.Broadcast(this);
	return true;
}


void ATile::Unbuild()
{
	// there is no Building
	if (!Building) return;
	if (Claimant)
	{
		Claimant->OnBuildingRemoved(Building);
	}
	// Remove Building related GameplayTags
	GameplayTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Building")));
	OnGameplayTagsChanged.Broadcast();
	// Destroy the Object
	Building = nullptr;
	// Set Graphics
	OnBuildingChanged.Broadcast(this);
	InitTileLayout();
}

// ---------------------------------------------------------
// Ecosystem and calculate Turn 

void ATile::CalculateTurn()
{
	int32 _;
	// apply TreeGrowthChange
	TreeGrowth->Add(TreeGrowthChange->Current, _);
	// grow trees
	if (TreeGrowth->Current > BalanceData->TreeGrowthThreshold)
	{
		const int32 TreeGrowCount = TreeGrowth->Current / BalanceData->TreeGrowthThreshold;
		Trees->Add(TreeGrowCount, _);
		TreeGrowth->Subtract(TreeGrowCount * BalanceData->TreeGrowthThreshold, _);
	}
	// apply ForageChange
	int32 ForageEffectiveChange = 0;
	Forage->Add(ForageChange->Current, ForageEffectiveChange);
	// wildlife starvation
	if (ForageEffectiveChange < 0)
	{
		Wildlife->Add(ForageEffectiveChange, _);
	}
	// apply WildlifeGrowthChange
	WildlifeGrowth->Add(WildlifeGrowthChange->Current, _);
	// grow Wildlife
	if (WildlifeGrowth->Current > BalanceData->WildlifeGrowthThreshold)
	{
		const int32 WildlifeGrowCount = WildlifeGrowth->Current / BalanceData->WildlifeGrowthThreshold;
		Wildlife->Add(WildlifeGrowCount, _);
		WildlifeGrowth->Subtract(WildlifeGrowCount * BalanceData->WildlifeGrowthThreshold, _);
	}
	if (!Building) return;
	// apply PopulationGrowthChange
	Building->PopContainer->Growth += Building->PopContainer->GrowthChange;
	// grow Population
	if (Building->PopContainer->Growth > Building->PopContainer->GrowthThreshold)
	{
		const int32 PopulationGrowCount = Building->PopContainer->Growth / Building->PopContainer->GrowthThreshold;
		Building->PopContainer->IncreaseSize(PopulationGrowCount);
		Building->PopContainer->Growth -= PopulationGrowCount * Building->PopContainer->GrowthThreshold;
	}
}

void ATile::CalculateTreeGrowthChange()
{
	TreeGrowthChange->SetCurrent(0);
	int32 _;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i])
		{
			TreeGrowthChange->Add(Neighbors[i]->Trees->Current, _);
		}
	}
	TreeGrowthChange->Add(Trees->Current, _);
}

void ATile::CalculateTreeGrowthChangeWithNeighbors(int32 Change)
{
	if (bFreezeGrowthChanges) return;

	CalculateTreeGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculateTreeGrowthChange();
	}
}

void ATile::CalculateForageChange()
{
	ForageChange->SetCurrent(0);
	int32 _;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i])
		{
			ForageChange->Add(Neighbors[i]->Trees->Current * BalanceData->ForagePerNeighboringTree, _);
			ForageChange->Add(Neighbors[i]->ForageChange->Current * BalanceData->ForagePerNeighboringForage, _);
		}
	}
	ForageChange->Add(Trees->Current * BalanceData->ForagePerTree, _);
	ForageChange->Add(ForageChange->Current * BalanceData->ForagePerForage, _);
	ForageChange->Subtract(Wildlife->Current, _);
}

void ATile::CalculateForageChangeWithNeighbors(int32 Change)
{
	if (bFreezeGrowthChanges) return;

	CalculateForageChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculateForageChange();
	}
}

void ATile::CalculateWildlifeGrowthChange()
{
	WildlifeGrowthChange->SetCurrent(0);
	int32 _;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i])
		{
			WildlifeGrowthChange->Add(Neighbors[i]->Wildlife->Current, _);
		}
	}
	WildlifeGrowthChange->Add(Wildlife->Current, _);
}

void ATile::CalculateWildlifeGrowthChangeWithNeighbors(int32 Change)
{
	if (bFreezeGrowthChanges) return;

	CalculateWildlifeGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculateWildlifeGrowthChange();
	}
}

void ATile::CalculatePopulationGrowthChange()
{
	if (!Building) return;

	Building->PopContainer->GrowthChange = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i] && Neighbors[i]->Building)
		{
			Building->PopContainer->GrowthChange += Neighbors[i]->Building->PopContainer->Population.Size;
		}
	}
	Building->PopContainer->GrowthChange += Building->PopContainer->Population.Size;
}

void ATile::CalculatePopulationGrowthChangeWithNeighbors(FPopulation Change)
{
	if (bFreezeGrowthChanges) return;

	CalculatePopulationGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculatePopulationGrowthChange();
	}
}

// -----------------------Graphics--------------------------

void ATile::InitHexagonMesh()
{
	SM_Hexagon->SetStaticMesh(HexagonMesh);
	if (HexagonMesh) UpdateHexagonMaterial();
}

// -----------------------Terrain---------------------------

void ATile::TerrainClientInit()
{
	if (!TileContent) InitTileContent();
	TileContent->SetTerrain(Terrain);
	UpdateHexagonMaterial();
}

void ATile::TerrainServerInit(const FTerrain& Terrain_)
{
	FTerrain OldTerrain = Terrain;
	Terrain = Terrain_;
	MARK_PROPERTY_DIRTY_FROM_NAME(ATile, Terrain, this);
	GameplayTags.RemoveTag(DA_Biomes->AllBiomes);
	GameplayTags.AddTag(DA_Biomes->EnumToTag[Terrain_.Biome]);
	TerrainClientInit();
	InitTileLayout();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ATile::UpdateHexagonMaterial()
{
	switch (Terrain.Biome)
	{
	case EBiome::Gras:
		SM_Hexagon->SetMaterial(0, DA_TileGraphics->M_Grass);
		break;
	case EBiome::Beach:
		SM_Hexagon->SetMaterial(0, DA_TileGraphics->M_Beach);
		break;
	case EBiome::Mountain:
		SM_Hexagon->SetMaterial(0, DA_TileGraphics->M_Mountain);
		break;
	case EBiome::Volcano:
		SM_Hexagon->SetMaterial(0, DA_TileGraphics->M_Volcano);
		break;
	}
}

// ------------------TileContent--------------------

void ATile::InitTileContent()
{
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	TileContent = NewObject<UTileContent>();
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
	DA_TileGraphics->TileLayouts->GetAllRows<FTileLayout>(_, AllRows);

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
	ApplySpawnChances(SL.Trees);
	ApplySpawnChances(SL.Forage);
	ApplySpawnChances(SL.Props);
	ApplySpawnChances(SL.Buildings);
	SetSpawnLayout(SL);
	Trees->SetMaximum(SpawnLayout.Trees.Num());
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
		if(DA_SpawnLayout->IsValidFor(GameplayTags)) PossibleLayouts.Add(DA_SpawnLayout);
	}
	if(PossibleLayouts.Num() <= 0) return nullptr;
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
