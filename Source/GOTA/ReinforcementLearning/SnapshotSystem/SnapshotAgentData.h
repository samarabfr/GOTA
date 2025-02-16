// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SnapshotAgentData.generated.h"

class ULearningAgentsNeuralNetwork;

USTRUCT()
struct GOTA_API FSnapshotNeuralNetworkData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FString Name = "Unnamed";

	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsNeuralNetwork* Encoder = nullptr;

	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsNeuralNetwork* Policy = nullptr;

	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsNeuralNetwork* Decoder = nullptr;

	UPROPERTY(EditDefaultsOnly)
	ULearningAgentsNeuralNetwork* Critic = nullptr;
};

UCLASS()
class GOTA_API USnapshotAgentData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FString AgentName = "Unnamed";

	UPROPERTY(EditDefaultsOnly)
	TArray<FSnapshotNeuralNetworkData> NeuralNetworks;

public:
	FString GetAgentName() const { return AgentName; }
	TArray<FString> GetAllNeuralNetworkNames();
	FSnapshotNeuralNetworkData GetNeuralNetworkByName(const FString& NeuralNetworkName);
};
