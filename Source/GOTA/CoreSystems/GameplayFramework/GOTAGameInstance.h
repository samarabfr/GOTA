// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GOTAGameInstance.generated.h"

class UTileAsset;

UCLASS()
class GOTA_API UGOTAGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// ------------------- LifeCycle -------------------

	virtual void Init() override;
	
	// ------------------- Ability Provider -------------------
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability Provider")
	UDataTable* AbilityRegister;

	public:
	UDataTable* GetAbilityRegister() const { return AbilityRegister; }

	// ------------------- Tile Asset Provider -------------------
private:
	UPROPERTY(EditDefaultsOnly, Category = "Tile Asset Provider")
	UTileAsset* DefaultTileAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Tile Asset Provider")
	UDataTable* TileAssetRegister;

public:
	UTileAsset* GetDefaultTileAsset() const { return DefaultTileAsset; }
	UDataTable* GetTileAssetRegister() const { return TileAssetRegister; }

	// ------------------- Tile Layout Provider -------------------
private:
	UPROPERTY(EditDefaultsOnly, Category = "Tile Layout Provider")
	UDataTable* TileLayoutRegister;

	UPROPERTY(EditDefaultsOnly, Category = "Tile Layout Provider")
	UDataTable* SpawnLayoutRegister;

public:
	UDataTable* GetTileLayoutRegister() const { return TileLayoutRegister; }
	UDataTable* GetSpawnLayoutRegister() const { return SpawnLayoutRegister; }
};
