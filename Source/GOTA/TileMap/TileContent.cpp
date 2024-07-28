#include "TileContent.h"
#include "Tile.h"
#include "TileAsset.h"
#include "GOTA/Faction/Building.h"
#include "GOTA/Faction/BuildingDataAsset.h"

ATileContent::ATileContent()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	MainBuilding = CreateDefaultSubobject<UStaticMeshComponent>("MainBuilding");
	MainBuilding->SetupAttachment(RootComponent);
}

void ATileContent::Init(ATile* Tile_)
{
	Tile = Tile_;
	Tile->Trees->OnChanged.AddDynamic(this, &ATileContent::UpdateTrees);
	UpdateTrees(Tile->Trees->Current);
}

void ATileContent::UpdateTrees(int32 Change)
{
	if (Change == 0) return;
	int32 Counter = 0;
	// increase the amount of visible trees
	if (Change > 0)
	{
		for (UStaticMeshComponent* Mesh : TreeMeshes)
		{
			if (!Mesh->IsVisible())
			{
				Mesh->SetVisibility(true);
				if (++Counter >= Change) return;
			}
		}
	}
	else
	{
		for (UStaticMeshComponent* Mesh : TreeMeshes)
		{
			if (Mesh->IsVisible())
			{
				Mesh->SetVisibility(false);
				if (--Counter <= Change) return;
			}
		}
	}
}

void ATileContent::UpdateBuildings()
{
	if (Tile->Building)
	{
		FString ContextString;
		TArray<FTileAsset*> BuildingTileAssets;
		BuildingAssets->GetAllRows<FTileAsset>(ContextString, BuildingTileAssets);
		// Activate Building Stuff
		MainBuilding->SetStaticMesh(Tile->Building->DataAsset->MainBuilding.StaticMesh);
		MainBuilding->SetVisibility(true);
		if (BuildingTileAssets.IsEmpty()) return;
		TArray<FTileAsset*> FoundBuildingAssets;
		FindRandomValidAssets(BuildingMeshes.Num(), BuildingTileAssets, FoundBuildingAssets);
		// Something went wrong while finding Building Assets
		if(FoundBuildingAssets.Num() != BuildingMeshes.Num()) return;
		for (int i = 0; i < BuildingMeshes.Num(); ++i)
		{
			BuildingMeshes[i]->SetStaticMesh(FoundBuildingAssets[i]->StaticMesh);
			BuildingMeshes[i]->SetVisibility(true);
		}
	}
	else
	{
		// Deactivate Building Stuff
		for (UStaticMeshComponent* BuildingMesh : BuildingMeshes)
		{
			BuildingMesh->SetVisibility(false);
		}
		MainBuilding->SetVisibility(false);
	}
}

void ATileContent::FindRandomValidAssets(const int32 Amount, const TArray<FTileAsset*>& Assets,
                                         TArray<FTileAsset*>& OutFoundAssets) const
{
	// First Filter Through the Input Array to find out which Assets are valid for this Tile
	TArray<FTileAsset*> PossibleAssets;
	for (FTileAsset* Asset : Assets)
	{
		bool bValid = true;
		for (FGameplayTagRule Rule : Asset->GameplayTagRules)
		{
			if (!Rule.IsValid(&Tile->GameplayTags))
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
