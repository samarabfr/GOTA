// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnLayoutDataAsset.h"
#include "GameFramework/Actor.h"
#include "TileLayout.generated.h"

USTRUCT(BlueprintType)
struct FTileLayout : public FTableRowBase
{
	GENERATED_BODY()

	FTileLayout();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMesh* HexagonMesh;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool HasRiver;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<bool> RiverConnections;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag LayoutTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<USpawnLayoutDataAsset*> SpawnLayouts;
};
