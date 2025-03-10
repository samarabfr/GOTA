// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotAgent.h"


// Add default functionality here for any ISnapshotAgent functions that are not pure virtual.
void ISnapshotAgent::SaveModel(const FString& ModelName)
{
}

void ISnapshotAgent::LoadModel(const FString& ModelName)
{
}

FString ISnapshotAgent::GetAgentName()
{
	return "Unnamed";
}
