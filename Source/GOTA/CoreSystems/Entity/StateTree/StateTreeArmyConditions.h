// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "StateTreeConditionBase.h"

#include "StateTreeArmyConditions.generated.h"

class AArmy;
struct FStateTreeDataView;

USTRUCT()
struct GOTA_API FSTC_CurrentTileIsValidForRecruitingInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};
STATETREE_POD_INSTANCEDATA(FSTC_CurrentTileIsValidForRecruitingInstanceData);

/**
 * Condition checking if current tile is valid for recruiting
 */
USTRUCT(DisplayName = "Current Tile is valid for recruiting")
struct GOTA_API FSTC_CurrentTileIsValidForRecruiting : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_CurrentTileIsValidForRecruitingInstanceData;

	FSTC_CurrentTileIsValidForRecruiting() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};
