// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat.h"
#include "GameBalance.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "CombatSystem.generated.h"

UCLASS(Blueprintable)
class GOTA_API UCombatSystem : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	UPROPERTY()
	TArray<ACombat*> Combats;

public:
	void RegisterCombat(ATile* Tile);
	void TriggerAllCombats();
};
