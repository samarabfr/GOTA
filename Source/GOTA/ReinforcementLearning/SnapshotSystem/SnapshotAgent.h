// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SnapshotAgent.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class USnapshotAgent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOTA_API ISnapshotAgent
{
	GENERATED_BODY()

	
public:
	virtual void SaveModel(const FString& ModelName);
	virtual void LoadModel(const FString& ModelName);
	virtual FString GetAgentName();
};
