#include "TileContent.h"
#include "Tile.h"

ATileContent::ATileContent()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	MainBuilding = CreateDefaultSubobject<UStaticMeshComponent>("MainBuilding");
}

void ATileContent::Init(ATile* Tile_)
{
	Tile = Tile_;
	Tile->Trees->OnChanged.AddDynamic(this, &ATileContent::UpdateTrees);
	UpdateTrees(Tile->Trees->Current);
}

void ATileContent::UpdateTrees(int32 Change)
{
	if(Change == 0) return;
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
	} else
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
