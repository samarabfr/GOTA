// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotAgents.h"

#include "SnapshotAgentData.h"

USnapshotAgentData* USnapshotAgents::GetAgentByName(const FString& AgentName)
{
	for (USnapshotAgentData* Agent : Agents)
	{
		if (Agent->GetAgentName() == AgentName)
		{
			return Agent;
		}
	}
	return nullptr;
}

TArray<FString> USnapshotAgents::GetAllAgentNames()
{
	TArray<FString> AgentNames;
	for (USnapshotAgentData* Agent : Agents)
	{
		AgentNames.Add(Agent->GetAgentName());
	}
	return AgentNames;
}
