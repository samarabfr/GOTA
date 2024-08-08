#include "TileContent.h"
#include "Tile.h"
#include "Components/StaticMeshComponent.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingDataAsset.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"

void UTileContent::Init(ATile* Tile_, AGS_Ingame* GameState_)
{
	GameState = GameState_;
	Tile = Tile_;
	Tile->OnSpawnPointLayoutChanged.AddDynamic(this, &UTileContent::OnSpawnPointLayoutChanged);
	Tile->OnGameplayTagsChanged.AddDynamic(this, &UTileContent::ValidateEverything);
	Tile->Trees->OnChanged.AddDynamic(this, &UTileContent::UpdateTrees);
	Tile->Forage->OnChanged.AddDynamic(this, &UTileContent::UpdateForage);
	Tile->OnBuildingChanged.AddDynamic(this, &UTileContent::ValidateBuildings);
	Tile->OnBuildingChanged.AddDynamic(this, &UTileContent::ValidateMainBuilding);
	OnSpawnPointLayoutChanged();
}

void UTileContent::SetRotation(FRotator Rotator)
{
	Rotation = Rotator;
	DespawnEverything();
	ValidateEverything();
}

void UTileContent::DespawnEverything()
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
	int8 RealChange = Tile->Trees->Current - CountHowManyAreSpawned;
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

void UTileContent::UpdateForage(int32 Change)
{
	int8 CountHowManyAreSpawned = 0;
	for (FTileAssetSpawn TileAssetSpawn : ForageTileAssetSpawns)
	{
		if (TileAssetSpawn.bIsSpawned) ++CountHowManyAreSpawned;
	}
	int8 RealChange = Tile->Forage->Current / 4 - CountHowManyAreSpawned;
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
	BringArrayToCorrectSize(TreeTileAssetSpawns, Tile->SpawnPointLayout.Trees.Num());
	SetSpawnPointsOnArray(TreeTileAssetSpawns, Tile->SpawnPointLayout.Trees);

	BringArrayToCorrectSize(PropTileAssetSpawns, Tile->SpawnPointLayout.Props.Num());
	SetSpawnPointsOnArray(PropTileAssetSpawns, Tile->SpawnPointLayout.Props);

	BringArrayToCorrectSize(BuildingTileAssetSpawns, Tile->SpawnPointLayout.Buildings.Num());
	SetSpawnPointsOnArray(BuildingTileAssetSpawns, Tile->SpawnPointLayout.Buildings);

	BringArrayToCorrectSize(ForageTileAssetSpawns, Tile->SpawnPointLayout.Forage.Num());
	SetSpawnPointsOnArray(ForageTileAssetSpawns, Tile->SpawnPointLayout.Forage);

	MainBuilding.SpawnPoint = Tile->SpawnPointLayout.MainBuilding;
	if (MainBuilding.TileAsset)
	{
		if (MainBuilding.TileAsset->RotationMode == ERotationMode::Random360Degree)
		{
			MainBuilding.SpawnPoint.Rotation = FMath::RandRange(0, 359);
		}
		else if (MainBuilding.TileAsset->RotationMode == ERotationMode::Random90Degree)
		{
			MainBuilding.SpawnPoint.Rotation = 90 * FMath::RandRange(0, 3);
		}
	}
	if (MainBuilding.bIsSpawned)
	{
		FTransform Transform = FTransform();
		Transform.SetLocation(MainBuilding.SpawnPoint.LocationOnTile + Tile->GetActorLocation());
		Transform.SetRotation(FRotator(0, MainBuilding.SpawnPoint.Rotation, 0).Quaternion());
		GameState->StaticMeshBatcher->UpdateStaticMeshTransform(
			MainBuilding.TileAsset->StaticMesh, MainBuilding.InstanceId, Transform);
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
	ShuffleTArray(SpawnPoints);
	for (int i = 0; i < Array.Num(); ++i)
	{
		Array[i].SpawnPoint = SpawnPoints[i];
		if (Array[i].TileAsset)
		{
			if (Array[i].TileAsset->RotationMode == ERotationMode::Random360Degree)
			{
				Array[i].SpawnPoint.Rotation = FMath::RandRange(0, 359);
			}
			else if (Array[i].TileAsset->RotationMode == ERotationMode::Random90Degree)
			{
				Array[i].SpawnPoint.Rotation = 90 * FMath::RandRange(0, 3);
			}
		}
		if (Array[i].bIsSpawned)
		{
			FTransform Transform = FTransform();
			Transform.SetLocation(Array[i].SpawnPoint.LocationOnTile + Tile->GetActorLocation());
			Transform.SetRotation(FRotator(0, Array[i].SpawnPoint.Rotation, 0).Quaternion());
			GameState->StaticMeshBatcher->UpdateStaticMeshTransform(
				Array[i].TileAsset->StaticMesh, Array[i].InstanceId, Transform);
		}
	}
}

void UTileContent::ValidateEverything()
{
	ValidateTileAssets(TreeTileAssetSpawns, Tile->DA_TileGraphics->TreeAssets);
	UpdateTrees(0);
	ValidateTileAssets(PropTileAssetSpawns, Tile->DA_TileGraphics->PropAssets);
	SpawnProps();
	ValidateTileAssets(BuildingTileAssetSpawns, Tile->DA_TileGraphics->BuildingAssets);
	ValidateBuildings();
	ValidateTileAssets(ForageTileAssetSpawns, Tile->DA_TileGraphics->ForageAssets);
	UpdateForage(0);
	ValidateMainBuilding();
}

void UTileContent::ValidateMainBuilding()
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

void UTileContent::SpawnProps()
{
	for (FTileAssetSpawn& TileAssetSpawn : PropTileAssetSpawns)
	{
		SpawnTileAsset(TileAssetSpawn);
	}
}

void UTileContent::ValidateBuildings()
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

void UTileContent::ValidateTileAssets(TArray<FTileAssetSpawn>& Array, const UDataTable* Assets)
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
			if (TileAssetSpawn.TileAsset)
			{
				if (TileAssetSpawn.TileAsset->RotationMode == ERotationMode::Random360Degree)
				{
					TileAssetSpawn.SpawnPoint.Rotation = FMath::RandRange(0, 359);
				}
				else if (TileAssetSpawn.TileAsset->RotationMode == ERotationMode::Random90Degree)
				{
					TileAssetSpawn.SpawnPoint.Rotation = 90 * FMath::RandRange(0, 3);
				}
			}
		}
	}
}

void UTileContent::SpawnTileAsset(FTileAssetSpawn& FTileAssetSpawn)
{
	// already spawned
	if (FTileAssetSpawn.bIsSpawned) return;
	FTransform Transform;
	FVector RotatedSpawnPointLocation = Rotation.RotateVector(FTileAssetSpawn.SpawnPoint.LocationOnTile);
	Transform.SetLocation(RotatedSpawnPointLocation + Tile->GetActorLocation());
	FRotator Rot = FRotator(0, FTileAssetSpawn.SpawnPoint.Rotation, 0) + Rotation;
	Transform.SetRotation(Rot.Quaternion());
	FTileAssetSpawn.InstanceId = GameState->StaticMeshBatcher->AddStaticMeshInstance(
		FTileAssetSpawn.TileAsset->StaticMesh, Transform);
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

void UTileContent::FindRandomValidAssets(const int32 Amount, const UDataTable* DataTable,
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
		int Count = FMath::RandRange(0, TotalBias - 1);
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
void UTileContent::ShuffleTArray(TArray<T>& Array)
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
