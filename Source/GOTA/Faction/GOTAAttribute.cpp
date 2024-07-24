#include "GOTAAttribute.h"
#include "Net/UnrealNetwork.h"

void UGOTAAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGOTAAttribute, Current);
}

bool UGOTAAttribute::IsSupportedForNetworking() const
{
	return true;
}

void UGOTAAttribute::OnChange()
{
	OnChanged.Broadcast(Current - OldValue);
	OldValue = Current;
}

int32 UGOTAAttribute::GetCurrent() const
{
	return Current;
}

void UGOTAAttribute::SetCurrent(int32 NewValue)
{
	Current = NewValue;
	OnChange();
}

void UGOTAAttribute::OnRep_Current()
{
	OnChange();
}


void UGOTAAttribute::Add(const int32 Addend, int32& Effective_Change)
{
	int32 const Before = Current;
	SetCurrent(Current + Addend);
	Effective_Change = Current - Before;
}

void UGOTAAttribute::Subtract(const int32 Subtrahend, int32& Effective_Change)
{
	int32 const Before = Current;
	SetCurrent(Current - Subtrahend);
	Effective_Change = Current - Before;
}

void UGOTAAttribute::Multiply(const float Factor, int32& Effective_Change)
{
	int32 const Before = Current;
	SetCurrent(Current * Factor);
	Effective_Change = Current - Before;
}