// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileAssetRegisterEntry.generated.h"

class UTileAsset;

USTRUCT(BlueprintType)
struct FTileAssetRegisterEntry : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UTileAsset* TileAsset = nullptr;
};
