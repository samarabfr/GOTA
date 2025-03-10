// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnLayoutDataAsset.h"
#include "SpawnLayoutStruct.h"
#include "GameFramework/Actor.h"
#include "TileLayout.generated.h"

UENUM(meta = (ScriptName = "ETileLayout"))
enum class ETileLayout : uint8
{
	Layout_01 UMETA(DisplayName = "Layout 01"),
	Layout_02 UMETA(DisplayName = "Layout 02"),
	Layout_11 UMETA(DisplayName = "Layout 11"),
	Layout_21 UMETA(DisplayName = "Layout 21"),
	Layout_22 UMETA(DisplayName = "Layout 22"),
	Layout_31 UMETA(DisplayName = "Layout 31"),
	Layout_32 UMETA(DisplayName = "Layout 32"),
	Layout_33 UMETA(DisplayName = "Layout 33")
};

USTRUCT()
struct FTileLayout : public FTableRowBase
{
	GENERATED_BODY()

	FTileLayout();

	UPROPERTY(EditDefaultsOnly)
	ETileLayout Layout;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag LayoutTag;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* HexagonMesh;

	UPROPERTY(EditDefaultsOnly)
	bool HasRiver;

	UPROPERTY(EditDefaultsOnly)
	TArray<bool> RiverConnections;

	UPROPERTY(EditDefaultsOnly)
	TArray<USpawnLayoutDataAsset*> SpawnLayouts;

	TArray<FSpawnLayoutStruct> SpawnLayoutStructs;
	
	/**
	 * returns a random valid Rotation of this TileLayout with the given RiverConnections
	 * 
	 * @param InRiverConnections RiverConnection array with Length = 6
	 * @return -1 if no valid connection found, 0-5 otherwise
	 */
	int32 GetValidRotation(const TArray<bool>& InRiverConnections) const;
};
