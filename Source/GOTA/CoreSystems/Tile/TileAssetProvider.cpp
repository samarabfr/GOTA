#include "TileAssetProvider.h"

#include "TileAsset.h"
#include "TileAssetRegisterEntry.h"
#include "GOTA/CoreSystems/GameplayFramework/GOTAGameInstance.h"

// ---------------------------------------- Lifecycle ----------------------------------------

void UTileAssetProvider::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	int32 LoadedCounter = 0;

	const UGOTAGameInstance* GameInstance = Cast<UGOTAGameInstance>(GetGameInstance());

	if (GameInstance->GetDefaultTileAsset())
	{
		DefaultTileAsset = GameInstance->GetDefaultTileAsset();
	}
	
	if (!GameInstance->GetTileAssetRegister()) return;
	GameInstance->GetTileAssetRegister()->ForeachRow<FTileAssetRegisterEntry>(
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
	UE_LOG(LogTemp, Log, TEXT("Loaded %d TileAssets"), LoadedCounter)
}

// ---------------------------------------- Utility ----------------------------------------
