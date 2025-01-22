#include "TileAssetProvider.h"

#include "TileAsset.h"
#include "TileAssetRegisterEntry.h"

// ---------------------------------------- Lifecycle ----------------------------------------

void UTileAssetProvider::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	int32 LoadedCounter = 0;

	const TCHAR* PathToDefaultTileAsset = TEXT("/Game/CoreSystems/Tile/TileAssets/DA_DefaultTileAsset");
	if (UTileAsset* LoadedDefaultTileAsset = LoadObject<UTileAsset>(nullptr, PathToDefaultTileAsset))
	{
		DefaultTileAsset = LoadedDefaultTileAsset;
	}

	const TCHAR* PathToTileAssetRegister = TEXT("/Game/CoreSystems/Tile/TileAssets/DT_TileAssetsRegister");
	if (const UDataTable* TileAssetRegister = LoadObject<UDataTable>(nullptr, PathToTileAssetRegister))
	{
		TileAssetRegister->ForeachRow<FTileAssetRegisterEntry>(
			TEXT("Load Tile Assets"),
			[&](const FName& RowName, const FTileAssetRegisterEntry& RowData)
			{
				if (RowData.TileAsset != nullptr)
				{
					switch (RowData.TileAsset->Category)
					{
					case ETileAssetCategory::MainBuilding:
						MainBuildingAssets.Add(RowData.TileAsset);
						break;

					case ETileAssetCategory::Building:
						BuildingAssets.Add(RowData.TileAsset);
						break;

					case ETileAssetCategory::Tree:
						TreeAssets.Add(RowData.TileAsset);
						break;

					case ETileAssetCategory::Prop:
						PropAssets.Add(RowData.TileAsset);
						break;

					case ETileAssetCategory::Forage:
						ForageAssets.Add(RowData.TileAsset);
						break;

					default:
						--LoadedCounter;
						break;
					}
					++LoadedCounter;
				}
			});
	}
	UE_LOG(LogTemp, Log, TEXT("Loaded %d TileAssets"), LoadedCounter)
}

// ---------------------------------------- Utility ----------------------------------------
