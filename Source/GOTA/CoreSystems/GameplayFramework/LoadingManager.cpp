// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingManager.h"

#include "GOTAGameInstance.h"
#include "PC_Ingame.h"
#include "PS_Ingame.h"
#include "Net/UnrealNetwork.h"

void ALoadingManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoadingManager, LoadingStatuses);
}

void ALoadingManager::Init()
{
	const UGOTAGameInstance* GI = Cast<UGOTAGameInstance>(GetGameInstance());
//	LoadingStatuses.SetNum(GI->PlayerCount);
}

void ALoadingManager::IncrementReplicationCount()
{
	LoadingStatuses[GOTAPlayerID]->IncreaseReplicationCount();
}
