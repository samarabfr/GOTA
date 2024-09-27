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
	if (!CurrentTile) return;
	UEcoValues* EcoValues = CurrentTile->EcoValues;
	Tree_Current->SetText(FText::AsNumber(EcoValues->GetTrees()));
	Tree_Max->SetText(FText::AsNumber(EcoValues->GetMaxTrees()));
	Tree_Growth->SetText(FText::Format(FText::FromString(TEXT("+{0}")), EcoValues->GetTreeGrowth()));
	Tree_Progress->SetPercent(EcoValues->GetTreeGrowthProgress() * 0.01f);

	Wildlife_Current->SetText(FText::AsNumber(EcoValues->GetWildlife()));
	Wildlife_Max->SetText(FText::AsNumber(EcoValues->GetMaxWildlife()));
	Wildlife_Growth->SetText(FText::Format(FText::FromString(TEXT("+{0}")), EcoValues->GetWildlifeGrowth()));
	Wildlife_Progress->SetPercent(EcoValues->GetWildlifeGrowthProgress() * 0.01f);

	Forage_Current->SetText(FText::AsNumber(EcoValues->GetForage()));
	Forage_Max->SetText(FText::AsNumber(EcoValues->GetMaxForage()));
	Forage_Growth->SetText(FText::Format(FText::FromString(TEXT("+{0}")), EcoValues->GetForageGrowth()));
	Forage_Progress->SetPercent(EcoValues->GetForageGrowthProgress() * 0.01f);
}

void UTileInfo::WatchTile(ATile* Tile)
{
	CurrentTile = Tile;
}
