// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileLayout.h"
#include "GameFramework/Actor.h"
#include "SpawnLayoutActor.generated.h"

/**
 * An Actor to design a spawn layout. Create a blueprint that inherits from this class to start designing.
 * Add TileAssetComponents in the Blueprint to add Components
 */
UCLASS()
class GOTA_API ASpawnLayoutActor : public AActor
{
	GENERATED_BODY()

public:
	ASpawnLayoutActor();

private:
	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor", meta = (ForceRebuildProperty))
	ETileLayout TileLayout = ETileLayout::Layout_01;
	
	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor")
	TArray<FGameplayTagRule> GameplayTagRules;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor")
	bool GuaranteedIfPossible = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "SpawnLayoutActor")
	FSpawnBias SpawnBias;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Hexagon;

public:
	UFUNCTION(CallInEditor)
	void SayHello()
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello!"));
	}

	ETileLayout GetTileLayout() { return TileLayout; }

	FSpawnLayoutStruct GetSpawnLayout();

};
