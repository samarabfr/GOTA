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
struct GOTA_API FSTC_IsCurrentTileBestWorkTileInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if the current tile is the best work tile
 */
USTRUCT(DisplayName = "Is current tile best work tile")
struct GOTA_API FSTC_IsCurrentTileBestWorkTile : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsCurrentTileBestWorkTileInstanceData;

	FSTC_IsCurrentTileBestWorkTile() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT()
struct GOTA_API FSTC_IsCurrentTilePriorityTileInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if the current tile is the priority tile
 */
USTRUCT(DisplayName = "Is current tile priority tile")
struct GOTA_API FSTC_IsCurrentTilePriorityTile : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsCurrentTilePriorityTileInstanceData;

	FSTC_IsCurrentTilePriorityTile() = default;

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
