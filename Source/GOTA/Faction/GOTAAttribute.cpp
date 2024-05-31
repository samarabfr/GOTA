#include "GOTAAttribute.h"
#include "Net/UnrealNetwork.h"

void UGOTAAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGOTAAttribute, Current);
}

void UGOTAAttribute::OnChange()
{
	OnChanged.Broadcast(OldValue - Current);
	OldValue = Current;
}

float UGOTAAttribute::GetCurrent() const
{
	return Current;
}

void UGOTAAttribute::SetCurrent(float NewValue)
{
	if(NewValue < 0) // New Value is negative, NOT ALLOWED
	{
		if((Current == 0))
		{
			// Current Value is already 0, nothing happens
			return;
		}
		// Current Value has to be set to 0;
		Current = 0;

	} else // New Value is a valid value for Current
	{
		Current = NewValue;
	}
	OnChange();
}

void UGOTAAttribute::OnRep_Current()
{
	OnChange();
}


void UGOTAAttribute::Add(const float Addend, float& Effective_Change)
{
	float const Before = Current;
	SetCurrent(Current + Addend);
	Effective_Change = Current - Before;
}

void UGOTAAttribute::Subtract(const float Subtrahend, float& Effective_Change)
{
	float const Before = Current;
	SetCurrent(Current - Subtrahend);
	Effective_Change = Current - Before;
}

void UGOTAAttribute::Multiply(const float Factor, float& Effective_Change)
{
	float const Before = Current;
	SetCurrent(Current * Factor);
	Effective_Change = Current - Before;
}