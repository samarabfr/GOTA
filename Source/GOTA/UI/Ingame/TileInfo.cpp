#include "TileInfo.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GOTA/CoreSystems/Tile/EcoValues.h"
#include "GOTA/CoreSystems/Tile/Tile.h"

void UTileInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTileInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!CurrentTile) return;
	UEcoValues* EcoValues = CurrentTile->EcoValues;
	Tree_Current->SetText(FText::AsNumber(EcoValues->GetTrees()));
	Tree_Max->SetText(FText::AsNumber(EcoValues->GetMaxTrees()));
	TreeGrowth->SetText(FText::Format(FText::FromString(TEXT("+{0}")), EcoValues->GetTreeGrowth()));
	Tree_Progress->SetPercent(EcoValues->GetTreeGrowthProgress());
}

void UTileInfo::WatchTile(ATile* Tile)
{
	CurrentTile = Tile;
}
