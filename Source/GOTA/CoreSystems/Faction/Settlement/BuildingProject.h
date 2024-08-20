// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementImportanceRatings.h"
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
	UPROPERTY( BlueprintReadOnly, Replicated, Category="Building Project")
	ASettlement* Builder;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building Project")
	UBuildingDataAsset* Data;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building Project")
	int32 Tier = -1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building Project")
	ATile* Tile;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building Project")
	FGameResources Cost;
	
	UFUNCTION(BlueprintCallable, Category="Building Project")
	bool IsPossible();

	UFUNCTION(BlueprintCallable, Category="Building Project")
	bool CanAfford() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Building Project")
	bool TryBuilding();
	
	float CalculateScore();

	int32 CalculateProjectTime();

	UFUNCTION(BlueprintCallable, Category="Building Project")
	void Init(ASettlement* Builder_, UBuildingDataAsset* Data_, int32 Tier_, ATile* Tile_);
};
