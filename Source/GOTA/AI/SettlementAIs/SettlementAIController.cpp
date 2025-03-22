// Fill out your copyright notice in the Description page of Project Settings.


#include "SettlementAIController.h"

#include "GOTA/Settlement/Settlement.h"


ASettlementAIController::ASettlementAIController()
{
	bReplicates = false;
	bAlwaysRelevant = false;
	bReplicateUsingRegisteredSubObjectList = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.5;
}

void ASettlementAIController::Delete()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

void ASettlementAIController::S_Init(bool RunTraining)
{
	bRunTraining = RunTraining;
}

void ASettlementAIController::Possess(ASettlement* Settlement)
{
	PossessedSettlement = Settlement;
}

ASettlement* ASettlementAIController::GetPossessedSettlement() const
{
	return PossessedSettlement.Get();
}

void ASettlementAIController::Log()
{
}
