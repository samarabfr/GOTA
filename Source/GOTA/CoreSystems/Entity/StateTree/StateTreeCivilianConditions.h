// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "StateTreeConditionBase.h"

#include "StateTreeCivilianConditions.generated.h"

class ACivilian;
struct FStateTreeDataView;


USTRUCT()
struct GOTA_API FSTC_IsPathValidCivilianInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if path is valid for civilian
 */
USTRUCT(DisplayName = "Is Path valid for Civilian")
struct GOTA_API FSTC_IsPathValidCivilian : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsPathValidCivilianInstanceData;

	FSTC_IsPathValidCivilian() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsPathEmptyCivilianInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if path is valid for civilian
 */
USTRUCT(DisplayName = "Is Path Empty for Civilian")
struct GOTA_API FSTC_IsPathEmptyCivilian : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsPathEmptyCivilianInstanceData;

	FSTC_IsPathEmptyCivilian() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_HasPriorityTileInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if civilian has priority tile
 */
USTRUCT(DisplayName = "Has priority tile")
struct GOTA_API FSTC_HasPriorityTile : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_HasPriorityTileInstanceData;

	FSTC_HasPriorityTile() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsCurrentTileTheTargetInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if the current tile is the target tile
 */
USTRUCT(DisplayName = "Is current tile the target")
struct GOTA_API FSTC_IsCurrentTileTheTarget : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsCurrentTileTheTargetInstanceData;

	FSTC_IsCurrentTileTheTarget() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_HasResourcesInInventoryInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if civilian has resources in its inventory
 */
USTRUCT(DisplayName = "Has resources in inventory")
struct GOTA_API FSTC_HasResourcesInInventory : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_HasResourcesInInventoryInstanceData;

	FSTC_HasResourcesInInventory() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsInventoryFulInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if civilian has a full inventory
 */
USTRUCT(DisplayName = "Is inventory full")
struct GOTA_API FSTC_IsInventoryFull : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsInventoryFulInstanceData;

	FSTC_IsInventoryFull() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsCurrentTileTheOriginBuildingInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if the current tile is the origin building
 */
USTRUCT(DisplayName = "Is current tile the origin building")
struct GOTA_API FSTC_IsCurrentTileTheOriginBuilding : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsCurrentTileTheOriginBuildingInstanceData;

	FSTC_IsCurrentTileTheOriginBuilding() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};
