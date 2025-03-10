#include "StartParameter.h"

#include "Net/UnrealNetwork.h"

void UStartParameter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStartParameter, IslandSize)
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
