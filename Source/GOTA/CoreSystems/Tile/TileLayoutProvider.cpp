// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLayoutProvider.h"

#include "SpawnLayoutActor.h"
#include "SpawnLayoutRegisterEntry.h"
#include "GOTA/CoreSystems/GameplayFramework/GOTAGameInstance.h"

void UTileLayoutProvider::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UGOTAGameInstance* GameInstance = Cast<UGOTAGameInstance>(GetGameInstance());

	UDataTable* TileLayoutRegister = GameInstance->GetTileLayoutRegister();
	if (TileLayoutRegister)
	{
		TileLayoutRegister->ForeachRow<FTileLayout>(
			TEXT("Load Tile Layouts"),
			[&](const FName& RowName, const FTileLayout& RowData)
			{
				int32 index = TileLayouts.Add(RowData);
				GenerateSpawnLayoutStructsFromDataAssets(TileLayouts[index]);
			});
	}

	UDataTable* SpawnLayoutRegister = GameInstance->GetSpawnLayoutRegister();
	if (SpawnLayoutRegister)
	{
		SpawnLayoutRegister->ForeachRow<FSpawnLayoutRegisterEntry>(
			TEXT("Load Spawn Layouts"),
			[&](const FName& RowName, const FSpawnLayoutRegisterEntry& RowData)
			{
				ASpawnLayoutActor* Actor = RowData.SpawnLayoutActorClass->GetDefaultObject<ASpawnLayoutActor>();
				ASpawnLayoutActor* SpawnedActor = GetWorld()->SpawnActor<ASpawnLayoutActor>(RowData.SpawnLayoutActorClass);
				GetTileLayout(SpawnedActor->GetTileLayout())->SpawnLayoutStructs.Add(SpawnedActor->GetSpawnLayoutStruct());
				SpawnedActor->Destroy();
			});
	}
}

void UTileLayoutProvider::GenerateSpawnLayoutStructsFromDataAssets(FTileLayout& TileLayout)
{
	for (USpawnLayoutDataAsset* SpawnLayoutDataAsset : TileLayout.SpawnLayouts)
	{
		FSpawnLayoutStruct NewStruct = FSpawnLayoutStruct();
		NewStruct.Name = SpawnLayoutDataAsset->Name;
		NewStruct.SpawnBias = SpawnLayoutDataAsset->SpawnBias;
		NewStruct.GameplayTagRules = SpawnLayoutDataAsset->GameplayTagRules;
		NewStruct.GuaranteedIfPossible = SpawnLayoutDataAsset->GuaranteedIfPossible;
		NewStruct.SpawnLayout = SpawnLayoutDataAsset->SpawnLayout;
		TileLayout.SpawnLayoutStructs.Add(NewStruct);
	}
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
