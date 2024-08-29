#include "StartParameter.h"

#include "Net/UnrealNetwork.h"

void UStartParameter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStartParameter, IslandSize)
	DOREPLIFETIME(UStartParameter, StartingColonialSettlements)
}

bool UStartParameter::IsSupportedForNetworking() const
{
	return true;
}

int32 UStartParameter::GetIslandSize()
{
	return IslandSize;
}

void UStartParameter::SetIslandSize(int32 NewValue)
{
	IslandSize = NewValue;
	OnChanged.Broadcast(this);
}

void UStartParameter::OnRep_IslandSize()
{
	OnChanged.Broadcast(this);
}

int32 UStartParameter::GetStartingColonialSettlements()
{
	return StartingColonialSettlements;
}

void UStartParameter::SetStartingColonialSettlements(int32 NewValue)
{
	StartingColonialSettlements = NewValue;
	OnChanged.Broadcast(this);
}

void UStartParameter::OnRep_StartingColonialSettlements()
{
	OnChanged.Broadcast(this);
}
