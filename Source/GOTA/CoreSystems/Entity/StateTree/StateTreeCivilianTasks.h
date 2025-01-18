// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeCivilianTasks.generated.h"

class ACivilian;

USTRUCT()
struct GOTA_API FCivilianInstanceData
{
	GENERATED_BODY()

	// automatically takes from the context
	UPROPERTY(EditAnywhere, Category=Context)
	TObjectPtr<ACivilian> CivilianRef = nullptr;
};

USTRUCT(DisplayName="Civilian move to next Tile")
struct GOTA_API FSTT_CivilianMoveToNextTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCivilianInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context,
	                       const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Work")
struct GOTA_API FSTT_Work : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCivilianInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context,
	                       const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(DisplayName="Find path to best work tile")
struct GOTA_API FSTT_FindPathToBestWorkTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCivilianInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Find path to priority tile")
struct GOTA_API FSTT_FindPathToPriorityTile : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCivilianInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(DisplayName="Find path to origin building")
struct GOTA_API FSTT_FindPathToOriginBuilding : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCivilianInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
