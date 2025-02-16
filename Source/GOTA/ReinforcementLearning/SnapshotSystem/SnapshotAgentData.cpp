// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotAgentData.h"

TArray<FString> USnapshotAgentData::GetAllNeuralNetworkNames()
{
	TArray<FString> NeuralNetworkNames;
	for (FSnapshotNeuralNetworkData NeuralNetwork : NeuralNetworks)
	{
		NeuralNetworkNames.Add(NeuralNetwork.Name);
	}
	return NeuralNetworkNames;
}

FSnapshotNeuralNetworkData USnapshotAgentData::GetNeuralNetworkByName(const FString& NeuralNetworkName)
{
	for (FSnapshotNeuralNetworkData& NeuralNetwork : NeuralNetworks)
	{
		if (NeuralNetwork.Name == NeuralNetworkName)
		{
			return NeuralNetwork;
		}
	}
	return FSnapshotNeuralNetworkData();
}
