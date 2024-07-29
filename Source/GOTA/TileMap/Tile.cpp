// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Core/LoadingManager.h"
#include "GOTA/Faction/Building.h"
#include "GOTA/Faction/Settlement.h"
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
	AGOTAGameState* GameState = GetWorld()->GetGameState<AGOTAGameState>();
	GameState->LoadingManager->IncrementReplicationCount();

	SpawnTileContent();
	TileContent->Init(this);

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
	ClaimantChanged();
}

void ATile::OnRep_Claimant(ASettlement* NewClaimant)
{
	ClaimantChanged();
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
	// Add Building related GameplayTags
	GameplayTags.AppendTags(BuildingDataAsset->GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	AddBuildingToReplication();
	// Set Graphics
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
	GameplayTags.RemoveTags(Building->DataAsset->GameplayTags);
	OnGameplayTagsChanged.Broadcast();
	// Destroy the Object
	Building = nullptr;
	// Set Graphics
	RecalculateTileLayout();
}

void ATile::AddBuildingToReplication()
{
	AddReplicatedSubObject(Building);
	AddReplicatedSubObject(Building->Population);
	AddReplicatedSubObject(Building->Production);

	// TODO: Not Here
	Building->Population->OnChanged.AddDynamic(this, &ATile::CalculatePopulationGrowthChangeWithNeighbors);
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
	// grow Wildlife
	if (Building->Population->Growth > Building->Population->GrowthThreshold)
	{
		const int32 PopulationGrowCount = Building->Population->Growth / Building->Population->GrowthThreshold;
		Building->Population += PopulationGrowCount;
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

	Building->Population->Growth = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (Neighbors[i] && Neighbors[i]->Building)
		{
			Building->Population->Growth += Neighbors[i]->Building->Population->Current;
		}
	}
	Building->Population->Growth += Building->Population->Current;
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
	if (TileContent) TileContent->SetActorRotation(FRotator(0, TileContentRotation, 0));
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
			Rotation = FindRiverConnectionRotation(TileLayout.RiverConnections);
		}
		else
		{
			Rotation = FMath::RandRange(0, 5);
		}
		SM_Hexagon->SetRelativeRotation(FRotator(0, Rotation * -60, 0));
		TileContent->SetActorRotation(FRotator(0, Rotation * -60, 0));
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
	if (IsValidTileLayout(&TileLayout))
	{
		RefreshTileLayout();
		return;
	}
	FTileLayout* NewLayout = FindNewValidTileLayout();
	if (!NewLayout) return;
	TileLayout = *NewLayout;
	// Select random SpawnPointLayout
	SpawnPointLayout = TileLayout.SpawnPointsLayouts[FMath::RandRange(0, TileLayout.SpawnPointsLayouts.Num() - 1)];
	OnSpawnPointLayoutChanged.Broadcast();
	Trees->SetMaximum(SpawnPointLayout.Trees.Num());
	Trees->SetCurrent(BalanceData->StartingTrees);
	RefreshTileLayout();
}

FTileLayout* ATile::FindNewValidTileLayout() const
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
	if (bIsRiver && FindRiverConnectionRotation(Layout->RiverConnections) < 0) return false;
	// This layout has no SpawnPointLayout
	if (Layout->SpawnPointsLayouts.IsEmpty()) return false;
	return true;
}

int32 ATile::FindRiverConnectionRotation(const TArray<bool> Connections) const
{
	if (Connections.Num() != 6) return -1;
	TArray<bool> RealConnections;
	for (int32 i = 0; i < 6; ++i)
	{
		RealConnections.Add(Neighbors[i] && Neighbors[i]->bIsRiver);
	}
	for (int32 Rotation = 0; Rotation < 6; ++Rotation)
	{
		bool ThisRotationWorks = true;
		for (int i = 0; i < 6; ++i)
		{
			if (RealConnections[i] != Connections[(i + Rotation) % 6])
			{
				ThisRotationWorks = false;
				break;
			}
		}
		if (ThisRotationWorks) return Rotation;
	}
	return -1;
}
