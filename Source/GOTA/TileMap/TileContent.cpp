#include "TileContent.h"
#include "Tile.h"
#include "Components/StaticMeshComponent.h"
#include "GOTA/Core/GOTAGameState.h"
#include "GOTA/Faction/Building.h"
#include "GOTA/Faction/BuildingDataAsset.h"
#include "GOTA/Faction/BuildingTierData.h"

ATileContent::ATileContent()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

void ATileContent::Init(ATile* Tile_)
{
	GameState = GetWorld()->GetGameState<AGOTAGameState>();
	Tile = Tile_;
	Tile->OnSpawnPointLayoutChanged.AddDynamic(this, &ATileContent::OnSpawnPointLayoutChanged);
	Tile->OnGameplayTagsChanged.AddDynamic(this, &ATileContent::ValidateAllTileAssets);
	Tile->Trees->OnChanged.AddDynamic(this, &ATileContent::UpdateTrees);
	Tile->Forage->OnChanged.AddDynamic(this, &ATileContent::UpdateForage);
	OnSpawnPointLayoutChanged();
}

void ATileContent::UpdateTrees(int32 Change)
{
	if (Change == 0) return;
	int32 Counter = 0;
	// increase the amount of visible trees
	if (Change > 0)
	{
		for (FTileAssetSpawn& TileAssetSpawn : TreeTileAssetSpawns)
		{
			if (!TileAssetSpawn.bIsSpawned)
			{
				SpawnTileAsset(TileAssetSpawn);
				if (++Counter >= Change) return;
			}
		}
	}
	else
	{
		for (FTileAssetSpawn& TileAssetSpawn : TreeTileAssetSpawns)
		{
			if (TileAssetSpawn.bIsSpawned)
			{
				DespawnTileAsset(TileAssetSpawn);
				if (--Counter <= Change) return;
			}
		}
	}
}

void ATileContent::UpdateForage(int32 Change)
{
	uint8 ForageSpawnAssetChange = Tile->Forage->Current / 4 - ForageAssetSpawnedCounter;
	if (ForageSpawnAssetChange == 0) return;
	int32 Counter = 0;
	// increase the amount of visible forage
	if (ForageSpawnAssetChange > 0)
	{
		for (FTileAssetSpawn& TileAssetSpawn : ForageTileAssetSpawns)
		{
			if (!TileAssetSpawn.bIsSpawned)
			{
				SpawnTileAsset(TileAssetSpawn);
				++ForageAssetSpawnedCounter;
				if (++Counter >= ForageSpawnAssetChange) return;
			}
		}
	}
	else
	{
		for (FTileAssetSpawn& TileAssetSpawn : ForageTileAssetSpawns)
		{
			if (TileAssetSpawn.bIsSpawned)
			{
				DespawnTileAsset(TileAssetSpawn);
				--ForageAssetSpawnedCounter;
				if (--Counter <= ForageSpawnAssetChange) return;
			}
		}
	}
}

void ATileContent::OnSpawnPointLayoutChanged()
{
	BringArrayToCorrectSize(TreeTileAssetSpawns, Tile->SpawnPointLayout.Trees.Num());
	SetSpawnPointsOnArray(TreeTileAssetSpawns, Tile->SpawnPointLayout.Trees);

	BringArrayToCorrectSize(PropTileAssetSpawns, Tile->SpawnPointLayout.Props.Num());
	SetSpawnPointsOnArray(PropTileAssetSpawns, Tile->SpawnPointLayout.Props);

	BringArrayToCorrectSize(BuildingTileAssetSpawns, Tile->SpawnPointLayout.Buildings.Num());
	SetSpawnPointsOnArray(BuildingTileAssetSpawns, Tile->SpawnPointLayout.Buildings);

	BringArrayToCorrectSize(ForageTileAssetSpawns, Tile->SpawnPointLayout.Forage.Num());
	SetSpawnPointsOnArray(ForageTileAssetSpawns, Tile->SpawnPointLayout.Forage);

	MainBuilding.SpawnPoint = Tile->SpawnPointLayout.MainBuilding;
	if(MainBuilding.bIsSpawned)
	{
		FTransform Transform = FTransform();
		Transform.SetLocation(MainBuilding.SpawnPoint.LocationOnTile + GetActorLocation());
		Transform.SetRotation(FRotator(0, MainBuilding.SpawnPoint.Rotation, 0).Quaternion());
		GameState->StaticMeshBatcher->UpdateStaticMeshTransform(
			MainBuilding.TileAsset->StaticMesh, MainBuilding.InstanceId, Transform);
	}

	ValidateAllTileAssets();
}

void ATileContent::BringArrayToCorrectSize(TArray<FTileAssetSpawn>& Array, int32 Size)
{
	if (Array.Num() < Size)
	{
		// Array has to grow
		Array.SetNum(Size);
	}
	// Array has to shrink
	while (Array.Num() > Size)
	{
		FTileAssetSpawn TileAssetSpawn = Array.Pop();
		DespawnTileAsset(TileAssetSpawn);
	}
}

void ATileContent::SetSpawnPointsOnArray(TArray<FTileAssetSpawn>& Array, TArray<FSpawnPoint> SpawnPoints)
{
	ShuffleTArray(SpawnPoints);
	for (int i = 0; i < Array.Num(); ++i)
	{
		Array[i].SpawnPoint = SpawnPoints[i];
		if (Array[i].TileAsset && Array[i].TileAsset->bRandomRotation)
		{
			Array[i].SpawnPoint.Rotation = FMath::RandRange(0, 359);
		}
		if (Array[i].bIsSpawned)
		{
			FTransform Transform = FTransform();
			Transform.SetLocation(Array[i].SpawnPoint.LocationOnTile + GetActorLocation());
			Transform.SetRotation(FRotator(0, Array[i].SpawnPoint.Rotation, 0).Quaternion());
			GameState->StaticMeshBatcher->UpdateStaticMeshTransform(
				Array[i].TileAsset->StaticMesh, Array[i].InstanceId, Transform);
		}
	}
}

void ATileContent::ValidateAllTileAssets()
{
	ValidateTileAssets(TreeTileAssetSpawns, Tile->DA_TileGraphics->TreeAssets);
	ValidateTrees();
	ValidateTileAssets(PropTileAssetSpawns, Tile->DA_TileGraphics->PropAssets);
	SpawnProps();
	ValidateTileAssets(BuildingTileAssetSpawns, Tile->DA_TileGraphics->BuildingAssets);
	ValidateBuildings();
	ValidateTileAssets(ForageTileAssetSpawns, Tile->DA_TileGraphics->ForageAssets);
	UpdateForage(0);
	ValidateMainBuilding();
}

void ATileContent::ValidateMainBuilding()
{
	if (Tile->Building)
	{
		MainBuilding.TileAsset = &(Tile->Building->DataAsset->GetTierData(Tile->Building->Tier)->MainBuildingAsset);
		SpawnTileAsset(MainBuilding);
	}
	else
	{
		DespawnTileAsset(MainBuilding);
	}
}

void ATileContent::ValidateTrees()
{
	int32 SpawnedTrees = 0;
	for (FTileAssetSpawn& TileAssetSpawn : TreeTileAssetSpawns)
	{
		if (TileAssetSpawn.bIsSpawned) SpawnedTrees++;
	}
	// correct amount of Trees, everything is good
	if (Tile->Trees->Current == SpawnedTrees) return;
	UpdateTrees(Tile->Trees->Current - SpawnedTrees);
}

void ATileContent::SpawnProps()
{
	for (FTileAssetSpawn& TileAssetSpawn : PropTileAssetSpawns)
	{
		SpawnTileAsset(TileAssetSpawn);
	}
}

void ATileContent::ValidateBuildings()
{
	if (Tile->Building)
	{
		for (FTileAssetSpawn& TileAssetSpawn : BuildingTileAssetSpawns)
		{
			SpawnTileAsset(TileAssetSpawn);
		}
	}
	else
	{
		for (FTileAssetSpawn& TileAssetSpawn : BuildingTileAssetSpawns)
		{
			DespawnTileAsset(TileAssetSpawn);
		}
	}
}

void ATileContent::ValidateTileAssets(TArray<FTileAssetSpawn>& Array, const UDataTable* Assets)
{
	// Remove Invalid and count how many new Assets we need
	int32 NewAssetsNeeded = 0;
	for (FTileAssetSpawn& TileAssetSpawn : Array)
	{
		if (!TileAssetSpawn.TileAsset)
		{
			NewAssetsNeeded++;
		}
		else if (!TileAssetSpawn.TileAsset->IsValidFor(Tile->GameplayTags))
		{
			NewAssetsNeeded++;
			DespawnTileAsset(TileAssetSpawn);
			TileAssetSpawn.TileAsset = nullptr;
		}
	}
	// Get new Assets and put them on the Array
	TArray<FTileAsset*> OutFoundAssets;
	FindRandomValidAssets(NewAssetsNeeded, Assets, OutFoundAssets);
	for (FTileAssetSpawn& TileAssetSpawn : Array)
	{
		if (!TileAssetSpawn.TileAsset)
		{
			TileAssetSpawn.TileAsset = OutFoundAssets.Pop();
			if (TileAssetSpawn.TileAsset->bRandomRotation)
			{
				TileAssetSpawn.SpawnPoint.Rotation = FMath::RandRange(0, 359);
			}
		}
	}
}

void ATileContent::SpawnTileAsset(FTileAssetSpawn& FTileAssetSpawn)
{
	// already spawned
	if (FTileAssetSpawn.bIsSpawned) return;
	FTransform Transform = FTransform();
	Transform.SetLocation(FTileAssetSpawn.SpawnPoint.LocationOnTile + GetActorLocation());
	Transform.SetRotation(FRotator(0, FTileAssetSpawn.SpawnPoint.Rotation, 0).Quaternion());
	FTileAssetSpawn.InstanceId = GameState->StaticMeshBatcher->AddStaticMeshInstance(
		FTileAssetSpawn.TileAsset->StaticMesh, Transform);
	FTileAssetSpawn.bIsSpawned = true;
}

void ATileContent::DespawnTileAsset(FTileAssetSpawn& FTileAssetSpawn)
{
	// not spawned
	if (!FTileAssetSpawn.bIsSpawned) return;
	GameState->StaticMeshBatcher->RemoveStaticMeshInstance(FTileAssetSpawn.TileAsset->StaticMesh,
	                                                       FTileAssetSpawn.InstanceId);
	FTileAssetSpawn.bIsSpawned = false;
}

void ATileContent::FindRandomValidAssets(const int32 Amount, const UDataTable* DataTable,
                                         TArray<FTileAsset*>& OutFoundAssets) const
{
	FString _;
	TArray<FTileAsset*> AllAssets;
	DataTable->GetAllRows(_, AllAssets);

	// First Filter Through the Input Array to find out which Assets are valid for this Tile
	TArray<FTileAsset*> PossibleAssets;
	for (FTileAsset* Asset : AllAssets)
	{
		bool bValid = true;
		for (FGameplayTagRule Rule : Asset->GameplayTagRules)
		{
			if (!Rule.IsValid(Tile->GameplayTags))
			{
				bValid = false;
				break; // Break if any rule is invalid
			}
		}
		if (bValid)
		{
			PossibleAssets.Add(Asset);
		}
	}
	if (PossibleAssets.IsEmpty()) return;
	// Calculate TotalBias for the weighted random selection
	int32 TotalBias = 0;
	for (FTileAsset* Asset : PossibleAssets)
	{
		TotalBias += Asset->SpawnBias;
	}
	// Randomly select the assets based on their spawn bias
	for (int32 i = 0; i < Amount; i++)
	{
		int Random = FMath::RandRange(0, TotalBias - 1);
		int Count = Random;
		for (FTileAsset* Asset : PossibleAssets)
		{
			if (Count < Asset->SpawnBias)
			{
				OutFoundAssets.Add(Asset);
				break;
			}
			Count -= Asset->SpawnBias;
		}
	}
}

template <typename T>
void ATileContent::ShuffleTArray(TArray<T>& Array)
{
	if (Array.Num() <= 1)
	{
		return;
	}
	// Create a random stream with a random seed
	FRandomStream RandomStream(FMath::Rand());

	for (int32 i = Array.Num() - 1; i > 0; i--)
	{
		int32 j = RandomStream.RandRange(0, i);
		Array.Swap(i, j);
	}
}
