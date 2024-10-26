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

USTRUCT()
struct GOTA_API FSTC_IsPathValidInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if current tile is valid for recruiting
 */
USTRUCT(DisplayName = "Is Path valid")
struct GOTA_API FSTC_IsPathValid : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsPathValidInstanceData;

	FSTC_IsPathValid() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_HasEnemyInGarrisonModeRangeInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if current tile is valid for recruiting
 */
USTRUCT(DisplayName = "Has enemy in Garrison mode range")
struct GOTA_API FSTC_HasEnemyInGarrisonModeRange : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_HasEnemyInGarrisonModeRangeInstanceData;

	FSTC_HasEnemyInGarrisonModeRange() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};
