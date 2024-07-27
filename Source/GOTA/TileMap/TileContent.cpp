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
		// TODO: Filter Rows for fitting GameplayTags
		// Activate Building Stuff
		MainBuilding->SetStaticMesh(Tile->Building->DataAsset->MainBuilding.StaticMesh);
		MainBuilding->SetVisibility(true);
		if (BuildingTileAssets.IsEmpty()) return;
		for (UStaticMeshComponent* BuildingMesh : BuildingMeshes)
		{
			BuildingMesh->SetStaticMesh(
				BuildingTileAssets[FMath::RandRange(0, BuildingTileAssets.Num() - 1)]->StaticMesh);
			BuildingMesh->SetVisibility(true);
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
