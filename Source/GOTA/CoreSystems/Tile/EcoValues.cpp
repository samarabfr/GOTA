#include "EcoValues.h"

void UEcoValues::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UEcoValues::IsSupportedForNetworking() const
{
	return true;
}
