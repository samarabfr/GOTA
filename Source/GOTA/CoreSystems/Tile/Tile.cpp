// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Settlement/Settlement.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/GameplayFramework/LoadingManager.h"
#include "Net/UnrealNetwork.h"

bool ATile::bFreezeGrowthChanges = false;

// ---------------------------------------------------------
// Initialisation and core variables

void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATile, HexCoords);
	DOREPLIFETIME(ATile, Claimant);
	DOREPLIFETIME(ATile, Trees);
	DOREPLIFETIME(ATile, TreeGrowth);
	DOREPLIFETIME(ATile, TreeGrowthChange);
	DOREPLIFETIME(ATile, Forage);
	DOREPLIFETIME(ATile, ForageChange);
	DOREPLIFETIME(ATile, Wildlife);
	DOREPLIFETIME(ATile, WildlifeGrowth);
	DOREPLIFETIME(ATile, WildlifeGrowthChange);
	DOREPLIFETIME(ATile, Building);
	DOREPLIFETIME(ATile, Neighbors);
	DOREPLIFETIME(ATile, AlliedTileEntity);
	DOREPLIFETIME(ATile, EnemyTileEntity);
	DOREPLIFETIME(ATile, bIsRiver);
	DOREPLIFETIME(ATile, Biome);
	DOREPLIFETIME(ATile, SpawnPointLayout);
	DOREPLIFETIME(ATile, GameplayTags);
	DOREPLIFETIME(ATile, TileContentRotation);
}

ATile::ATile()
{
	// initialize neighbor array
	for (int i = 0; i < 6; ++i)
	{
		Neighbors.Add(nullptr);
	}

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SM_Hexagon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Hexagon"));
	SM_Hexagon->SetupAttachment(RootComponent);

	// Replication stuff
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	bAlwaysRelevant = true;

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

	// Get the GameState
	AGS_Ingame* GameState = GetWorld()->GetGameState<AGS_Ingame>();
	GameState->LoadingManager->IncrementReplicationCount();

	TileContent = NewObject<UTileContent>();
	TileContent->Init(this, GameState);
	TileContent->SetRotation(FRotator(0, TileContentRotation, 0));
	UpdateHexagonMaterial();

	if (HasAuthority())
	{
		AddReplicatedSubObject(Trees);
		AddReplicatedSubObject(TreeGrowth);
		AddReplicatedSubObject(TreeGrowthChange);
		AddReplicatedSubObject(Forage);
		AddReplicatedSubObject(ForageChange);
		AddReplicatedSubObject(Wildlife);
		AddReplicatedSubObject(WildlifeGrowth);
		AddReplicatedSubObject(WildlifeGrowthChange);
	}
}

void ATile::Init()
{
	Trees->OnChanged.AddDynamic(this, &ATile::CalculateTreeGrowthChangeWithNeighbors);
	Forage->OnChanged.AddDynamic(this, &ATile::CalculateForageChangeWithNeighbors);
	Forage->SetMaximum(BalanceData->MaxForage);
	Forage->SetCurrent(BalanceData->StartingForage);
	Wildlife->OnChanged.AddDynamic(this, &ATile::CalculateWildlifeGrowthChangeWithNeighbors);
	Wildlife->SetMaximum(BalanceData->MaxWildlife);
	Wildlife->SetCurrent(BalanceData->StartingWildlife);
	RecalculateTileLayout();
	Trees->SetCurrent(BalanceData->StartingTrees);
	SetBiome(Biome);
}

void ATile::OnRep_GameplayTags()
{
	OnGameplayTagsChanged.Broadcast();
}

void ATile::SetIsRiver(bool IsRiver)
{
	bIsRiver = IsRiver;
	RecalculateTileLayout();
	for (ATile* Tile : Neighbors)
	{
		if (Tile) Tile->RecalculateTileLayout();
	}
}

void ATile::SetBiome(EBiome NewBiome)
{
	GameplayTags.RemoveTag(DA_Biomes->AllBiomes);
	GameplayTags.AddTag(DA_Biomes->EnumToTag[NewBiome]);
	OnGameplayTagsChanged.Broadcast();
	Biome = NewBiome;
	RecalculateTileLayout();
}

bool ATile::IsWalkable(EAffiliation Affiliation) const
{
	if (Affiliation == EAffiliation::Ally)
	{
		return !EnemyTileEntity;
	}
	if (Affiliation == EAffiliation::Enemy)
	{
		return !AlliedTileEntity;
	}
	return false;
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
		Building->OnClaim(Claimant);
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
		Building->OnUnclaim(Claimant);
		Claimant->OnBuildingRemoved(Building);
	}
	Claimant->LostClaim(this);
	GameplayTags.RemoveTags(Claimant->GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	Claimant = nullptr;
}

void ATile::OnRep_Building()
{
	OnBuildingChanged.Broadcast();
}

// ---------------------------------------------------------
// Building

bool ATile::TryBuild(UBuildingDataAsset* BuildingDataAsset)
{
	// There is already a Building, can't build here
	if (Building) return false;
	// Create Building Object
	Building = NewObject<UBuilding>(this, BuildingDataAsset->BuildingClass);
	Building->OnBuild(this);
	if (Claimant)
	{
		Building->OnClaim(Claimant);
		Claimant->OnBuildingAdded(Building);
	}
	int32 _;
	Building->Population->ChangeMaximum(BuildingDataAsset->TierOne.Housing, _);
	Building->Production->SetupWithTierData(&BuildingDataAsset->TierOne);
	// Add Building related GameplayTags
	GameplayTags.AppendTags(BuildingDataAsset->TierOne.GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	// Replication stuff
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->Population);
	AddReplicatedSubObject(Building->Production);
	// Population stuff
	Building->Population->OnChanged.AddDynamic(this, &ATile::CalculatePopulationGrowthChangeWithNeighbors);
	CalculatePopulationGrowthChangeWithNeighbors();
	// Set Graphics
	OnBuildingChanged.Broadcast();
	RecalculateTileLayout();
	return true;
}

void ATile::Unbuild()
{
	// there is no Building
	if (!Building) return;
	Building->OnUnbuild(this);
	if (Claimant)
	{
		Building->OnUnclaim(Claimant);
		Claimant->OnBuildingRemoved(Building);
	}
	// Remove Building related GameplayTags
	GameplayTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Building")));
	OnGameplayTagsChanged.Broadcast();
	// Destroy the Object
	Building = nullptr;
	// Set Graphics
	OnBuildingChanged.Broadcast();
	RecalculateTileLayout();
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
	Building->Population->Growth += Building->Population->GrowthChange;
	// grow Population
	if (Building->Population->Growth > Building->Population->GrowthThreshold)
	{
		const int32 PopulationGrowCount = Building->Population->Growth / Building->Population->GrowthThreshold;
		Building->Population->ChangePopulation(PopulationGrowCount, _);
		Building->Population->Growth -= PopulationGrowCount * Building->Population->GrowthThreshold;
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

	Building->Population->GrowthChange = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i] && Neighbors[i]->Building)
		{
			Building->Population->GrowthChange += Neighbors[i]->Building->Population->Current;
		}
	}
	Building->Population->GrowthChange += Building->Population->Current;
}

void ATile::CalculatePopulationGrowthChangeWithNeighbors()
{
	if (bFreezeGrowthChanges) return;

	CalculatePopulationGrowthChange();
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i]) Neighbors[i]->CalculatePopulationGrowthChange();
	}
}

// ---------------------------------------------------------
// TileLayout & TileContent and graphics relevant

void ATile::OnRep_SpawnPointLayout()
{
	OnSpawnPointLayoutChanged.Broadcast();
}

void ATile::OnRep_TileContentRotation()
{
	if (TileContent) TileContent->SetRotation(FRotator(0, TileContentRotation, 0));
}

void ATile::RefreshTileLayout()
{
	if (SM_Hexagon->GetStaticMesh() != TileLayout.HexagonMesh)
	{
		SM_Hexagon->SetStaticMesh(TileLayout.HexagonMesh);
		UpdateHexagonMaterial();
		MaterialBiome = Biome;
		int32 Rotation = 0;
		if (bIsRiver)
		{
			Rotation = FindAValidRiverConnectionRotation(TileLayout.RiverConnections);
		}
		else
		{
			Rotation = FMath::RandRange(0, 5);
		}
		SM_Hexagon->SetRelativeRotation(FRotator(0, Rotation * -60, 0));
		TileContent->SetRotation(FRotator(0, Rotation * -60, 0));
		TileContentRotation = Rotation * -60;
	}
	if (MaterialBiome != Biome)
	{
		UpdateHexagonMaterial();
		MaterialBiome = Biome;
	}
}

void ATile::RecalculateTileLayout()
{
	UpdateRiverConnections();
	if (IsValidTileLayout(&TileLayout))
	{
		RefreshTileLayout();
		return;
	}
	FTileLayout* NewLayout = FindNewValidTileLayout();
	if (!NewLayout) return;
	TileLayout = *NewLayout;
	// Select random SpawnPointLayout
	FSpawnPointLayout SPL = TileLayout.SpawnPointsLayouts[FMath::RandRange(0, TileLayout.SpawnPointsLayouts.Num() - 1)];
	ApplySpawnChances(SPL.Trees);
	ApplySpawnChances(SPL.Forage);
	ApplySpawnChances(SPL.Props);
	ApplySpawnChances(SPL.Buildings);
	SpawnPointLayout = SPL;
	OnSpawnPointLayoutChanged.Broadcast();
	Trees->SetMaximum(SpawnPointLayout.Trees.Num());
	Trees->SetCurrent(BalanceData->StartingTrees);
	RefreshTileLayout();
}

void ATile::ApplySpawnChances(TArray<FSpawnPoint>& SpawnPoints)
{
	// Check for Spawnpoints to remove
	for (int i = SpawnPoints.Num() - 1; i >= 0; --i)
	{
		if (SpawnPoints[i].SpawnChance != 100 && SpawnPoints[i].SpawnChance <= FMath::RandRange(0, 99))
		{
			SpawnPoints.RemoveAt(i);
		}
	}
}

FTileLayout* ATile::FindNewValidTileLayout()
{
	FString ContextString;
	TArray<FTileLayout*> AllRows;
	DA_TileGraphics->TileLayouts->GetAllRows<FTileLayout>(ContextString, AllRows);
	TArray<FTileLayout*> PossibleLayouts;
	for (FTileLayout* Row : AllRows)
	{
		if (IsValidTileLayout(Row))
		{
			PossibleLayouts.Add(Row);
		}
	}
	// No valid Layout found, so we use Default
	if (PossibleLayouts.IsEmpty())
	{
		FName DefaultName = "Default";
		UE_LOG(LogTemp, Warning, TEXT("Had to Default TileLayout on (%d:%d)"), HexCoords.Q, HexCoords.R);
		return DA_TileGraphics->TileLayouts->FindRow<FTileLayout>(DefaultName, ContextString);
	}
	return PossibleLayouts[FMath::RandRange(0, PossibleLayouts.Num() - 1)];
}

bool ATile::IsValidTileLayout(const FTileLayout* Layout) const
{
	// Tile has River but Row doesn't allow that
	if (bIsRiver != Layout->HasRiver) return false;
	// Tile has no Building but Row doesn't allow that
	if (!Building && !Layout->AllowNoBuilding) return false;
	// Tile has Building but Row doesn't allow that
	if (Building && !Layout->AllowBuilding) return false;
	// Row doesn't allow this biome
	if (!Layout->AllowedBiomes.Contains(Biome)) return false;
	// Is a river but can't find a working Rotation
	if (bIsRiver && FindAValidRiverConnectionRotation(Layout->RiverConnections) < 0) return false;
	// This layout has no SpawnPointLayout
	if (Layout->SpawnPointsLayouts.IsEmpty()) return false;
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
			if (RiverConnections[i] != Connections[(i + Rotation) % 6])
			{
				ThisRotationWorks = false;
				break;
			}
		}
		if (ThisRotationWorks) return Rotation;
	}
	return -1;
}

void ATile::UpdateRiverConnections()
{
	// Reset The Array
	RiverConnections.Empty();
	RiverConnections.SetNum(6);
	if (!bIsRiver) return;
	TArray<int32> PossibleNullConnections;
	int32 RealCount = 0;
	for (int32 i = 0; i < 6; ++i)
	{
		RiverConnections[i] = Neighbors[i] && Neighbors[i]->bIsRiver;
		if (RiverConnections[i]) ++RealCount;
		if (!Neighbors[i]) PossibleNullConnections.Add(i);
	}
	if (RealCount == 0) return;
	// we are under 3 connections and have possible null connects
	while (RealCount < 3 && PossibleNullConnections.Num() > 0)
	{
		int32 NullConnection = PossibleNullConnections[FMath::RandRange(0, PossibleNullConnections.Num() - 1)];
		PossibleNullConnections.Remove(NullConnection);
		RiverConnections[NullConnection] = true;
		RealCount++;
	}
}
