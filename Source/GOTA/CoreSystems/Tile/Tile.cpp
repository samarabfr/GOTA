// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
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
	
	DOREPLIFETIME(ATile, GameplayTags);
	DOREPLIFETIME(ATile, Building);
	DOREPLIFETIME(ATile, Claimant);
	DOREPLIFETIME(ATile, AlliedEntity);
	DOREPLIFETIME(ATile, EnemyEntity);
}

ATile::ATile()
{
	// Replication Setup
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
	NetUpdateFrequency = 1.0f;

	// Tick Setup
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.5f;

	// initialize neighbor array
	Neighbors.SetNumZeroed(6);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SM_Hexagon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Hexagon"));
	SM_Hexagon->SetupAttachment(RootComponent);

	EcoValues = CreateDefaultSubobject<UEcoValues>(TEXT("EcoValues"));
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();
}

void ATile::ServerInit()
{
	AddReplicatedSubObject(EcoValues);

	EcoValues->OnTreesChanged.AddDynamic(this, &ATile::AddTreesToNeighbors);
	EcoValues->OnWildlifeChanged.AddDynamic(this, &ATile::AddWildlifeToNeighbors);
	EcoValues->OnForageChanged.AddDynamic(this, &ATile::AddForageToNeighbors);
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
	Building->Population->ChangeMaxSize(BuildingDataAsset->Housing);
	// Add Building related GameplayTags
	GameplayTags.AppendTags(BuildingDataAsset->Tags);
	OnGameplayTagsChanged.Broadcast();
	// Replication stuff
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->Population);
	// Set Graphics
	OnBuildingChanged.Broadcast(this);
	InitTileLayout();
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

// -------------------Ecosystem-------------------------

void ATile::GOTATick()
{
	if(LastTick < 0)
	{
		LastTick =  GetWorld()->GetTimeSeconds();
		return;
	}
	double DeltaSeconds = GetWorld()->GetTimeSeconds() - LastTick;
	if (HasAuthority())
		EcoValues->ServerTick(DeltaSeconds);
	else
		EcoValues->ClientTick(DeltaSeconds);
	LastTick = GetWorld()->GetTimeSeconds();
}

void ATile::AddTreesToNeighbors(int32 Change)
{
	ForceNetUpdate();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->EcoValues->AddNeighborTrees(Change);
	}
}

void ATile::AddWildlifeToNeighbors(int32 Change)
{
	ForceNetUpdate();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->EcoValues->AddNeighborWildlife(Change);
	}
}

void ATile::AddForageToNeighbors(int32 Change)
{
	ForceNetUpdate();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->EcoValues->AddNeighborForage(Change);
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
	EcoValues->Init(Terrain.Biome);
}

void ATile::TerrainServerInit(const FTerrain& Terrain_)
{
	FTerrain OldTerrain = Terrain;
	Terrain = Terrain_;
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
		if (DA_SpawnLayout->IsValidFor(GameplayTags)) PossibleLayouts.Add(DA_SpawnLayout);
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
