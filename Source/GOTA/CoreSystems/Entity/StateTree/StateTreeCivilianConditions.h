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
struct GOTA_API FSTC_IsCurrentTileValidForWorkInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

/**
 * Condition checking if current tile is valid for work
 */
USTRUCT(DisplayName = "Is current tile valid for work")
struct GOTA_API FSTC_IsCurrentTileValidForWork : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTC_IsCurrentTileValidForWorkInstanceData;

	FSTC_IsCurrentTileValidForWork() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};
