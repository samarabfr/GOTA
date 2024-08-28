#pragma once

#include "EcoValues.generated.h"

UCLASS(Blueprintable)
class GOTA_API UEcoValues : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	
};
