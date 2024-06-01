// Fill out your copyright notice in the Description page of Project Settings.


#include "GOTAAttributeLimited.h"
#include "Net/UnrealNetwork.h"

void UGOTAAttributeLimited::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGOTAAttributeLimited, Maximum);
}

void UGOTAAttributeLimited::SetCurrent(float NewValue)
{
	if (NewValue < 0) // New Value is negative, NOT ALLOWED
	{
		if ((Current == 0))
		{
			// Current Value is already 0, nothing happens
			return;
		}
		// Current Value has to be set to 0;
		Current = 0;
	}
	else if (NewValue > Maximum) // New Value is too big, NOT ALLOWED
	{
		if (Current == Maximum)
		{
			// Current Value is already on Maximum, nothing happens
			return;
		}
		// Current Value has to be set to Maximum
		Current = Maximum;
	}
	else // New Value is a valid value for Current
	{
		Current = NewValue;
	}
	OnChange();
}

float UGOTAAttributeLimited::GetMaximum() const
{
	return Maximum;
}

void UGOTAAttributeLimited::SetMaximum(float NewValue)
{
	Maximum = FMath::Max(0, NewValue);
	OnChanged.Broadcast(0);
}

void UGOTAAttributeLimited::OnRep_Maximum()
{
	OnChange();
}

void UGOTAAttributeLimited::AddMaximum(const float Addend, float& Effective_Change)
{
	float const Before = Maximum;
	SetMaximum(Maximum + Addend);
	Effective_Change = Maximum - Before;
}

void UGOTAAttributeLimited::SubtractMaximum(const float Subtrahend, float& Effective_Change)
{
	float const Before = Maximum;
	SetMaximum(Maximum - Subtrahend);
	Effective_Change = Maximum - Before;
}

void UGOTAAttributeLimited::MultiplyMaximum(const float Factor, float& Effective_Change)
{
	float const Before = Maximum;
	SetMaximum(Maximum * Factor);
	Effective_Change = Maximum - Before;
}
