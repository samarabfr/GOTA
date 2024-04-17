// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Net/UnrealNetwork.h"

//Unreal Engine Mystery Code
void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATile, Claimant);
	DOREPLIFETIME(ATile, Nature);
	DOREPLIFETIME(ATile, MaxNature);
}

// Constructor
ATile::ATile()
{
	IsWalkable = true;
	IsClaimable = true;
}

//====================================================================
//--------------------Claimant
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::OnRep_Claimant(ASettlement* NewClaimant)
{
	ClaimantChanged();
}

ASettlement* ATile::GetClaimant()
{
	return Claimant;
}

void ATile::SetClaimant(ASettlement* NewClaimant)
{
	Claimant = NewClaimant;
	ClaimantChanged();
}

//====================================================================
//--------------------Nature
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddNature(const float Addend, float& Effective_Change)
{
	float Before = Nature;
	SetNature(Nature + Addend);
	Effective_Change = Nature - Before;
}

void ATile::MultiplyNature(const float Factor, float& Effective_Change)
{
	float Before = Nature;
	SetNature(Nature * Factor);
	Effective_Change = Nature - Before;
}

void ATile::OnRep_Nature(float NewNature)
{
	OnNatureChanged.Broadcast(NewNature);
}

void ATile::SetNature(float NewNature)
{
	Nature = FMath::Min(FMath::Max(NewNature, 0.0f), MaxNature);
	OnNatureChanged.Broadcast(Nature);
}

float ATile::GetNature()
{
	return Nature;
}


//====================================================================
//--------------------MaxNature
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddMaxNature(const float Addend, float& Effective_Change)
{
	float Before = MaxNature;
	SetMaxNature(MaxNature + Addend);
	Effective_Change = MaxNature - Before;
}

void ATile::MultiplyMaxNature(const float Factor, float& Effective_Change)
{
	float Before = MaxNature;
	SetMaxNature(MaxNature * Factor);
	Effective_Change = MaxNature - Before;
}

void ATile::OnRep_MaxNature(float NewMaxNature)
{
	OnMaxNatureChanged.Broadcast(NewMaxNature);
}

void ATile::SetMaxNature(float NewMaxNature)
{
	MaxNature = FMath::Max(NewMaxNature, 0.0f);
	OnMaxNatureChanged.Broadcast(MaxNature);
}

float ATile::GetMaxNature()
{
	return MaxNature;
}
