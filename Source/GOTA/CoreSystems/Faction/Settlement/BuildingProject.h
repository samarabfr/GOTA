// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingDataAsset.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "BuildingProject.generated.h"

class ASettlement;

UCLASS(Blueprintable)
class GOTA_API UBuildingProject : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building")
	ASettlement* Builder;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building")
	UBuildingDataAsset* Data;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building")
	int32 Tier = -1;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building")
	ATile* Tile;

	UFUNCTION(BlueprintCallable, Category="Building")
	bool IsPossible() const;

	UFUNCTION(BlueprintCallable, Category="Building")
	bool CanAfford() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Building")
	bool TryBuilding();

	UFUNCTION(BlueprintCallable, Category="Building")
	void Init(ASettlement* Builder_, UBuildingDataAsset* Data_, int32 Tier_, ATile* Tile_);
};
