// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "UObject/Interface.h"
#include "RL_BuildingSelectorAgent.generated.h"

class ASettlement;
class UBuildingSettings;
class ATile;
// This class does not need to be modified.
UINTERFACE()
class URL_BuildingSelectorAgent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOTA_API IRL_BuildingSelectorAgent
{
	GENERATED_BODY()

public:
	virtual ASettlement* GetSettlement();
	virtual TArray<UBuildingSettings*> GetAvailableBuildings();
	virtual void SelectBuilding(UBuildingSettings* Building);
	virtual EAffiliation GetAffiliation();
	virtual TArray<int32> GetMilestonesReached();
	virtual void IncrementMilestone(int32 MilestoneIndex);
};
