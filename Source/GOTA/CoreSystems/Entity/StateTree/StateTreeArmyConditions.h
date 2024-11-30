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
 * Condition checking if path is valid
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
 * Condition checking if army has enemy in garrison range
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

USTRUCT()
struct GOTA_API FSTC_HasEnemyOnNeighboringTileInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if army has an enemy on neighboring tile
 */
USTRUCT(DisplayName = "Has enemy on neighboring tile")
struct GOTA_API FSTC_HasEnemyOnNeighboringTile : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_HasEnemyOnNeighboringTileInstanceData;

	FSTC_HasEnemyOnNeighboringTile() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsOnEnemyBuildingInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if army is on an enemy building
 */
USTRUCT(DisplayName = "Is on enemy building")
struct GOTA_API FSTC_IsOnEnemyBuilding : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsOnEnemyBuildingInstanceData;

	FSTC_IsOnEnemyBuilding() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsBuildingProtectedInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if building on current tile is protected
 */
USTRUCT(DisplayName = "Is building protected")
struct GOTA_API FSTC_IsBuildingProtected : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsBuildingProtectedInstanceData;

	FSTC_IsBuildingProtected() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsOnGuardTileInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if army is on the guard tile
 */
USTRUCT(DisplayName = "Is on guard tile")
struct GOTA_API FSTC_IsOnGuardTile : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsOnGuardTileInstanceData;

	FSTC_IsOnGuardTile() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_HasEnemyInGuardTileRangeInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if enemy is in guard tile range
 */
USTRUCT(DisplayName = "Has enemy in guard tile range")
struct GOTA_API FSTC_HasEnemyInGuardTileRange : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_HasEnemyInGuardTileRangeInstanceData;

	FSTC_HasEnemyInGuardTileRange() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_HasGuardTileInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<AArmy> ArmyRef = nullptr;
};

/**
 * Condition checking if enemy is in guard tile range
 */
USTRUCT(DisplayName = "Has guard tile")
struct GOTA_API FSTC_HasGuardTile : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_HasGuardTileInstanceData;

	FSTC_HasGuardTile() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};
