// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Net/UnrealNetwork.h"

//Unreal Engine Mystery Code
void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATile, Claimant);
	DOREPLIFETIME(ATile, Trees);
	DOREPLIFETIME(ATile, MaxTrees);
	DOREPLIFETIME(ATile, Forage);
	DOREPLIFETIME(ATile, MaxForage);
	DOREPLIFETIME(ATile, Wildlife);
	DOREPLIFETIME(ATile, MaxWildlife);
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
//--------------------Trees
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddTrees(const float Addend, float& Effective_Change)
{
	float Before = Trees;
	SetTrees(Trees + Addend);
	Effective_Change = Trees - Before;
}

void ATile::MultiplyTrees(const float Factor, float& Effective_Change)
{
	float Before = Trees;
	SetTrees(Trees * Factor);
	Effective_Change = Trees - Before;
}

void ATile::OnRep_Trees(float NewTrees)
{
	OnTreesChanged.Broadcast(NewTrees);
}

void ATile::SetTrees(float NewTrees)
{
	Trees = FMath::Min(FMath::Max(NewTrees, 0.0f), MaxTrees);
	OnTreesChanged.Broadcast(Trees);
}

float ATile::GetTrees()
{
	return Trees;
}


//====================================================================
//--------------------MaxTrees
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddMaxTrees(const float Addend, float& Effective_Change)
{
	float Before = MaxTrees;
	SetMaxTrees(MaxTrees + Addend);
	Effective_Change = MaxTrees - Before;
}

void ATile::MultiplyMaxTrees(const float Factor, float& Effective_Change)
{
	float Before = MaxTrees;
	SetMaxTrees(MaxTrees * Factor);
	Effective_Change = MaxTrees - Before;
}

void ATile::OnRep_MaxTrees(float NewMaxTrees)
{
	OnMaxTreesChanged.Broadcast(NewMaxTrees);
}

void ATile::SetMaxTrees(float NewMaxTrees)
{
	MaxTrees = FMath::Max(NewMaxTrees, 0.0f);
	OnMaxTreesChanged.Broadcast(MaxTrees);
}

float ATile::GetMaxTrees()
{
	return MaxTrees;
}

//====================================================================
//--------------------Forage
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddForage(const float Addend, float& Effective_Change)
{
	float Before = Forage;
	SetForage(Forage + Addend);
	Effective_Change = Forage - Before;
}

void ATile::MultiplyForage(const float Factor, float& Effective_Change)
{
	float Before = Forage;
	SetForage(Forage * Factor);
	Effective_Change = Forage - Before;
}

void ATile::OnRep_Forage(float NewForage)
{
	OnForageChanged.Broadcast(NewForage);
}

void ATile::SetForage(float NewForage)
{
	Forage = FMath::Min(FMath::Max(NewForage, 0.0f), MaxForage);
	OnForageChanged.Broadcast(Forage);
}

float ATile::GetForage()
{
	return Forage;
}


//====================================================================
//--------------------MaxForage
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddMaxForage(const float Addend, float& Effective_Change)
{
	float Before = MaxForage;
	SetMaxForage(MaxForage + Addend);
	Effective_Change = MaxForage - Before;
}

void ATile::MultiplyMaxForage(const float Factor, float& Effective_Change)
{
	float Before = MaxForage;
	SetMaxForage(MaxForage * Factor);
	Effective_Change = MaxForage - Before;
}

void ATile::OnRep_MaxForage(float NewMaxForage)
{
	OnMaxForageChanged.Broadcast(NewMaxForage);
}

void ATile::SetMaxForage(float NewMaxForage)
{
	MaxForage = FMath::Max(NewMaxForage, 0.0f);
	OnMaxForageChanged.Broadcast(MaxForage);
}

float ATile::GetMaxForage()
{
	return MaxForage;
}

//====================================================================
//--------------------Wildlife
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddWildlife(const float Addend, float& Effective_Change)
{
	float Before = Wildlife;
	SetWildlife(Wildlife + Addend);
	Effective_Change = Wildlife - Before;
}

void ATile::MultiplyWildlife(const float Factor, float& Effective_Change)
{
	float Before = Wildlife;
	SetWildlife(Wildlife * Factor);
	Effective_Change = Wildlife - Before;
}

void ATile::OnRep_Wildlife(float NewWildlife)
{
	OnWildlifeChanged.Broadcast(NewWildlife);
}

void ATile::SetWildlife(float NewWildlife)
{
	Wildlife = FMath::Min(FMath::Max(NewWildlife, 0.0f), MaxWildlife);
	OnWildlifeChanged.Broadcast(Wildlife);
}

float ATile::GetWildlife()
{
	return Wildlife;
}


//====================================================================
//--------------------MaxWildlife
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void ATile::AddMaxWildlife(const float Addend, float& Effective_Change)
{
	float Before = MaxWildlife;
	SetMaxWildlife(MaxWildlife + Addend);
	Effective_Change = MaxWildlife - Before;
}

void ATile::MultiplyMaxWildlife(const float Factor, float& Effective_Change)
{
	float Before = MaxWildlife;
	SetMaxWildlife(MaxWildlife * Factor);
	Effective_Change = MaxWildlife - Before;
}

void ATile::OnRep_MaxWildlife(float NewMaxWildlife)
{
	OnMaxWildlifeChanged.Broadcast(NewMaxWildlife);
}

void ATile::SetMaxWildlife(float NewMaxWildlife)
{
	MaxWildlife = FMath::Max(NewMaxWildlife, 0.0f);
	OnMaxWildlifeChanged.Broadcast(MaxWildlife);
}

float ATile::GetMaxWildlife()
{
	return MaxWildlife;
}