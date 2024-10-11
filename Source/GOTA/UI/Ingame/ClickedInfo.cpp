#include "ClickedInfo.h"

#include "BuildingInfo.h"
#include "TileInfo.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void UClickedInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UClickedInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UClickedInfo::WatchActor(AActor* Actor)
{
	ATile* Tile = Cast<ATile>(Actor);
	if (Tile)
	{
		TileInfo->WatchTile(Tile);
		if (Tile->GetBuilding())
			
			BuildingInfo->WatchBuilding(Tile->GetBuilding());
	}
}
