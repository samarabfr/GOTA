// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DistanceUtilsSettings.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "GOTA/CoreSystems/Tile/HexCoords.h"
#include "DistanceUtils.generated.h"

UCLASS()
class GOTA_API ADistanceUtils : public AActor
{
	GENERATED_BODY()
	ADistanceUtils();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	UDistanceUtilsSettings* Settings;

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateDistanceToTiles();
	
private:
	FHexCoords CurrentCoords;
	TArray<FHexCoords> CurrentCoordsInRange;
	TWeakObjectPtr<AGS_Ingame> GameState;
};
