#pragma once

#include "Engine/DataAsset.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "AbilitySettings.generated.h"

class AAbility;
class AGuardian;

UCLASS()
class GOTA_API UAbilitySettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbility> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly)
	EAbilityCategory Category;
	
	UPROPERTY(EditDefaultsOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	// Cooldown in seconds
	UPROPERTY(EditDefaultsOnly)
	float Cooldown;
};
