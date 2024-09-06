#include "TileContent.h"
#include "Tile.h"
#include "Algo/RandomShuffle.h"
#include "Components/StaticMeshComponent.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingDataAsset.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"

void UTileContent::Init(ATile* Tile_, AGS_Ingame* GameState_)
{
	GameState = GameState_;
	Tile = Tile_;
	Tile->OnGameplayTagsChanged.AddDynamic(this, &UTileContent::ValidateEverything);
	Tile->EcoValues->OnTreesChanged.AddDynamic(this, &UTileContent::UpdateTrees);
	Tile->EcoValues->OnForageChanged.AddDynamic(this, &UTileContent::UpdateForage);
	OnSpawnPointLayoutChanged();
}

void UTileContent::SetRotation(FRotator Rotator)
{
	Rotation = Rotator;
	DespawnEveryTileAsset();
	ValidateEverything();
}

void UTileContent::SetTerrain(const FTerrain& Terrain_)
{
	Terrain = Terrain_;
	DespawnEveryTileAsset();
	NullEveryTileAsset();
	ValidateEverything();
}

void UTileContent::DespawnEveryTileAsset()
{
	for (FTileAssetSpawn& TileAsset : TreeTileAssetSpawns)
	{
		DespawnTileAsset(TileAsset);
	}
	for (FTileAssetSpawn& TileAsset : PropTileAssetSpawns)
	{
		DespawnTileAsset(TileAsset);
	}
	for (FTileAssetSpawn& TileAsset : BuildingTileAssetSpawns)
	{
		DespawnTileAsset(TileAsset);
	}
	for (FTileAssetSpawn& TileAsset : ForageTileAssetSpawns)
	{
		DespawnTileAsset(TileAsset);
	}
	DespawnTileAsset(MainBuilding);
}

void UTileContent::UpdateTrees(int32 Change)
{
	int8 CountHowManyAreSpawned = 0;
	for (FTileAssetSpawn TileAssetSpawn : TreeTileAssetSpawns)
	{
		if (TileAssetSpawn.bIsSpawned) ++CountHowManyAreSpawned;
	}
	int8 RealChange = Tile->EcoValues->GetTrees() - CountHowManyAreSpawned;
	if (RealChange == 0) return;
	int32 Counter = 0;
	// increase the amount of visible trees
	if (RealChange > 0)
	{
		for (FTileAssetSpawn& TileAssetSpawn : TreeTileAssetSpawns)
		{
			if (!TileAssetSpawn.bIsSpawned)
			{
				SpawnTileAsset(TileAssetSpawn);
				if (++Counter >= RealChange) return;
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
				if (--Counter <= RealChange) return;
			}
		}
	}
}

void UTileContent::NullEveryTileAsset()
{
	for (FTileAssetSpawn& TileAssetSpawn : TreeTileAssetSpawns)
	{
		TileAssetSpawn.TileAsset = nullptr;
	}
	for (FTileAssetSpawn& TileAssetSpawn : PropTileAssetSpawns)
	{
		TileAssetSpawn.TileAsset = nullptr;
	}
	for (FTileAssetSpawn& TileAssetSpawn : BuildingTileAssetSpawns)
	{
		TileAssetSpawn.TileAsset = nullptr;
	}
	for (FTileAssetSpawn& TileAssetSpawn : ForageTileAssetSpawns)
	{
		TileAssetSpawn.TileAsset = nullptr;
	}
	MainBuilding.TileAsset = nullptr;
}

void UTileContent::UpdateForage(int32 Change)
{
	int8 CountHowManyAreSpawned = 0;
	for (FTileAssetSpawn TileAssetSpawn : ForageTileAssetSpawns)
	{
		if (TileAssetSpawn.bIsSpawned) ++CountHowManyAreSpawned;
	}
	int8 RealChange = Tile->EcoValues->GetForage() / 4 - CountHowManyAreSpawned;
	if (RealChange == 0) return;
	int32 Counter = 0;
	// increase the amount of visible forage
	if (RealChange > 0)
	{
		for (FTileAssetSpawn& TileAssetSpawn : ForageTileAssetSpawns)
		{
			if (!TileAssetSpawn.bIsSpawned)
			{
				SpawnTileAsset(TileAssetSpawn);
				if (++Counter >= RealChange) return;
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
				if (--Counter <= RealChange) return;
			}
		}
	}
}

void UTileContent::OnSpawnPointLayoutChanged()
{
	BringArrayToCorrectSize(TreeTileAssetSpawns, Tile->SpawnLayout.Trees.Num());
	SetSpawnPointsOnArray(TreeTileAssetSpawns, Tile->SpawnLayout.Trees);

	BringArrayToCorrectSize(PropTileAssetSpawns, Tile->SpawnLayout.Props.Num());
	SetSpawnPointsOnArray(PropTileAssetSpawns, Tile->SpawnLayout.Props);

	BringArrayToCorrectSize(BuildingTileAssetSpawns, Tile->SpawnLayout.Buildings.Num());
	SetSpawnPointsOnArray(BuildingTileAssetSpawns, Tile->SpawnLayout.Buildings);

	BringArrayToCorrectSize(ForageTileAssetSpawns, Tile->SpawnLayout.Forage.Num());
	SetSpawnPointsOnArray(ForageTileAssetSpawns, Tile->SpawnLayout.Forage);

	MainBuilding.SpawnPoint = Tile->SpawnLayout.MainBuilding;
	if (MainBuilding.TileAsset)
	{
		MainBuilding.SpawnPoint.Rotation += MainBuilding.TileAsset->GetRotationAfterMode();
	}
	if (MainBuilding.bIsSpawned)
	{
		FTransform T = FTransform();
		CalculateTransform(MainBuilding.SpawnPoint, T);
		GameState->StaticMeshBatcher->UpdateStaticMeshTransform(
			MainBuilding.TileAsset->StaticMesh, MainBuilding.InstanceId, T);
	}

	ValidateEverything();
}

void UTileContent::BringArrayToCorrectSize(TArray<FTileAssetSpawn>& Array, int32 Size)
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

void UTileContent::SetSpawnPointsOnArray(TArray<FTileAssetSpawn>& Array, TArray<FSpawnPoint> SpawnPoints)
{
	Algo::RandomShuffle(SpawnPoints);
	for (int i = 0; i < Array.Num(); ++i)
	{
		Array[i].SpawnPoint = SpawnPoints[i];
		if (Array[i].TileAsset)
		{
			Array[i].SpawnPoint.Rotation += Array[i].TileAsset->GetRotationAfterMode();
		}
		if (Array[i].bIsSpawned)
		{
			FTransform T = FTransform();
			CalculateTransform(Array[i].SpawnPoint, T);
			GameState->StaticMeshBatcher->UpdateStaticMeshTransform(
				Array[i].TileAsset->StaticMesh, Array[i].InstanceId, T);
		}
	}
}

void UTileContent::ValidateEverything()
{
	ValidateTileAssets(TreeTileAssetSpawns, Tile->DA_TileGraphics->TreeTileAssets);
	UpdateTrees(0);
	ValidateTileAssets(PropTileAssetSpawns, Tile->DA_TileGraphics->PropTileAssets);
	SpawnProps();
	ValidateTileAssets(BuildingTileAssetSpawns, Tile->DA_TileGraphics->BuildingTileAssets);
	ValidateBuildings(Tile);
	ValidateTileAssets(ForageTileAssetSpawns, Tile->DA_TileGraphics->ForageTileAssets);
	UpdateForage(0);
	ValidateMainBuilding(Tile);
}

void UTileContent::ValidateMainBuilding(ATile* Tile_)
{
	if (Tile->Building)
	{
		MainBuilding.TileAsset = Tile->DA_TileGraphics->DefaultTileAsset;
		SpawnTileAsset(MainBuilding);
	}
	else
	{
		DespawnTileAsset(MainBuilding);
	}
}

void UTileContent::SpawnProps()
{
	for (FTileAssetSpawn& TileAssetSpawn : PropTileAssetSpawns)
	{
		SpawnTileAsset(TileAssetSpawn);
	}
}

void UTileContent::ValidateBuildings(ATile* Tile_)
{
	if (Tile_->Building)
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

void UTileContent::ValidateTileAssets(TArray<FTileAssetSpawn>& Array, const TArray<UTileAssetDA*>& Assets)
{
	// Remove Invalid and count how many new Assets we need
	int32 NewAssetsNeeded = 0;
	for (FTileAssetSpawn& TileAssetSpawn : Array)
	{
		if (!TileAssetSpawn.TileAsset)
		{
			NewAssetsNeeded++;
		}
		else if (!TileAssetSpawn.TileAsset->IsValidFor(Tile->GameplayTags)
			|| TileAssetSpawn.SpawnPoint.ForcedAssets.Num() > 0
			&& !TileAssetSpawn.SpawnPoint.ForcedAssets.Contains(TileAssetSpawn.TileAsset))
		{
			NewAssetsNeeded++;
			DespawnTileAsset(TileAssetSpawn);
			TileAssetSpawn.TileAsset = nullptr;
		}
		if (!TileAssetSpawn.TileAsset && TileAssetSpawn.SpawnPoint.ForcedAssets.Num() > 0)
		{
			// Needs asset and is using Forced Asset
			TileAssetSpawn.TileAsset = TileAssetSpawn.SpawnPoint.ForcedAssets
				[FMath::RandRange(0, TileAssetSpawn.SpawnPoint.ForcedAssets.Num() - 1)];
			TileAssetSpawn.SpawnPoint.Rotation += TileAssetSpawn.TileAsset->GetRotationAfterMode();
		}
	}
	// Get new Assets and put them on the Array
	TArray<UTileAssetDA*> OutFoundAssets;
	FindRandomValidAssets(NewAssetsNeeded, Assets, OutFoundAssets);
	if (OutFoundAssets.Num() != NewAssetsNeeded) return;
	for (FTileAssetSpawn& TileAssetSpawn : Array)
	{
		if (!TileAssetSpawn.TileAsset)
		{
			TileAssetSpawn.TileAsset = OutFoundAssets.Pop();
			TileAssetSpawn.SpawnPoint.Rotation += TileAssetSpawn.TileAsset->GetRotationAfterMode();
		}
	}
}

void UTileContent::SpawnTileAsset(FTileAssetSpawn& FTileAssetSpawn)
{
	// already spawned
	if (FTileAssetSpawn.bIsSpawned || !FTileAssetSpawn.TileAsset) return;
	FTransform T = FTransform();
	CalculateTransform(FTileAssetSpawn.SpawnPoint, T);
	FTileAssetSpawn.InstanceId = GameState->StaticMeshBatcher->AddStaticMeshInstance(
		FTileAssetSpawn.TileAsset->StaticMesh, T);
	FTileAssetSpawn.bIsSpawned = true;
}

void UTileContent::DespawnTileAsset(FTileAssetSpawn& FTileAssetSpawn)
{
	// not spawned
	if (!FTileAssetSpawn.bIsSpawned) return;
	GameState->StaticMeshBatcher->RemoveStaticMeshInstance(FTileAssetSpawn.TileAsset->StaticMesh,
	                                                       FTileAssetSpawn.InstanceId);
	FTileAssetSpawn.bIsSpawned = false;
}

void UTileContent::FindRandomValidAssets(const int32 Amount, const TArray<UTileAssetDA*>& AssetArray,
                                         TArray<UTileAssetDA*>& OutFoundAssets) const
{
	if (Amount <= 0) return;
	// First Filter Through the Input Array to find out which Assets are valid for this Tile
	TArray<UTileAssetDA*> PossibleAssets;
	for (UTileAssetDA* Asset : AssetArray)
	{
		if (Asset->IsValidFor(Tile->GameplayTags))
		{
			PossibleAssets.Add(Asset);
		}
	}
	// Use Default if no Possible Assets could be found
	if (PossibleAssets.IsEmpty())
		PossibleAssets.Add(Tile->DA_TileGraphics->DefaultTileAsset);
	// Calculate TotalBias for the weighted random selection
	int32 TotalBias = 0;
	for (UTileAssetDA* Asset : PossibleAssets)
	{
		TotalBias += Asset->GetBiasAfterMultipliers(Terrain);
	}
	// Randomly select the assets based on their spawn bias
	for (int32 i = 0; i < Amount; i++)
	{
		int32 Count = FMath::RandRange(0, TotalBias - 1);
		for (UTileAssetDA* Asset : PossibleAssets)
		{
			int32 SpawnBias = Asset->GetBiasAfterMultipliers(Terrain);
			if (Count < SpawnBias)
			{
				OutFoundAssets.Add(Asset);
				break;
			}
			Count -= SpawnBias;
		}
	}
}

void UTileContent::CalculateTransform(const FSpawnPoint& SpawnPoint, FTransform& Transform)
{
	FVector RotatedSpawnPointLocation = Rotation.RotateVector(SpawnPoint.LocationOnTile);
	Transform.SetLocation(RotatedSpawnPointLocation + Tile->GetActorLocation());
	FRotator Rot = FRotator(0, SpawnPoint.Rotation, 0) + Rotation;
	Transform.SetRotation(Rot.Quaternion());
}
