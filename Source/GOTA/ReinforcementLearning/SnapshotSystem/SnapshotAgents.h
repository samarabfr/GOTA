// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SnapshotAgents.generated.h"

class USnapshotAgentData;
/**
 * 
 */
UCLASS()
class GOTA_API USnapshotAgents : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<USnapshotAgentData*> Agents;

	UPROPERTY(EditDefaultsOnly)
	FFilePath SnapshotsFolderFilePath;

public:
	USnapshotAgentData* GetAgentByName(const FString& AgentName);
	TArray<FString> GetAllAgentNames();
	FFilePath GetSnapshotsFolderFilePath() const { return SnapshotsFolderFilePath; }
};
