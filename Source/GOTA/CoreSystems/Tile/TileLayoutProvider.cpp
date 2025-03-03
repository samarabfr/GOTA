// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLayoutProvider.h"

#include "GOTA/CoreSystems/GameplayFramework/GOTAGameInstance.h"

void UTileLayoutProvider::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UGOTAGameInstance* GameInstance = Cast<UGOTAGameInstance>(GetGameInstance());

	UDataTable* TileLayoutRegister = GameInstance->GetTileLayoutRegister();
	if (!TileLayoutRegister) return;

	TileLayoutRegister->ForeachRow<FTileLayout>(
		TEXT("Load Tile Layouts"),
		[&](const FName& RowName, const FTileLayout& RowData)
		{
			TileLayouts.Add(RowData);
		});
}

FTileLayout* UTileLayoutProvider::GetTileLayout(ETileLayout Layout)
{
	for (FTileLayout& TileLayout : TileLayouts)
	{
		if (TileLayout.Layout == Layout) return &TileLayout;
	}
	return nullptr;
}

FTileLayout* UTileLayoutProvider::GetFittingTileLayout(const FTerrain& Terrain)
{
	for (FTileLayout& TileLayout : TileLayouts)
	{
		if (!Terrain.bIsRiver && !TileLayout.HasRiver)
		{
			return &TileLayout;
		}
		if (Terrain.bIsRiver && TileLayout.HasRiver && TileLayout.GetValidRotation(Terrain.RiverConnections) >= 0)
		{
			return &TileLayout;
		}
	}
	return nullptr;
}
