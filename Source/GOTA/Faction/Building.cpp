// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Net/UnrealNetwork.h"

void UBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuilding, Population);
}

bool UBuilding::IsSupportedForNetworking() const
{
	return true;
}