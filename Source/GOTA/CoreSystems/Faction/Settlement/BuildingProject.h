// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Building/BuildingDataAsset.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "BuildingProject.generated.h"

class AColony;

UCLASS(Blueprintable)
class GOTA_API UBuildingProject : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

public:
	UPROPERTY( BlueprintReadOnly, Replicated, Category="Building Project")
	AColony* Builder;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Building Project")
	UBuildingDataAsset* Data;

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
	void Init(AColony* Builder_, UBuildingDataAsset* Data_, ATile* Tile_);
};
