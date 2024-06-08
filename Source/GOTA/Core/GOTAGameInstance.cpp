// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAGameInstance.h"

#include "Net/UnrealNetwork.h"

void UGOTAGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGOTAGameInstance, IslandRadius);
}
