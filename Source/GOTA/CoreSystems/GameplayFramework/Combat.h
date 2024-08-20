// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat.generated.h"

UCLASS()
class GOTA_API ACombat : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	TArray<ATile*> Tiles;

	UPROPERTY()
	TArray<ATile*> Sources;

	void AddSource(ATile* Tile);
	
	bool ShouldMerge(ATile* Tile);

	
};
