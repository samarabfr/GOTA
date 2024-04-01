// Fill out your copyright notice in the Description page of Project Settings.


#include "Settlement.h"
#include "Net/UnrealNetwork.h"

ASettlement::ASettlement()
{
	Attributes = NewObject<USettlementAttributes>();
}

void ASettlement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASettlement, Attributes);
}

USettlementAttributes* ASettlement::GetAttributes()
{
	return Attributes;
}
