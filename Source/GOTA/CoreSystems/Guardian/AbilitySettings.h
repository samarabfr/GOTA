#pragma once

#include "Engine/DataAsset.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "AbilitySettings.generated.h"

class AAbility;
class AGuardian;

UCLASS()
class GOTA_API UAbilitySettings : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbility> Class;

	UPROPERTY(EditDefaultsOnly)
	EAbilityCategory Category;

	UPROPERTY(EditDefaultsOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	float Cooldown;

public:
	TSubclassOf<AAbility> GetAbilityClass() const { return Class; }
	EAbilityCategory GetCategory() const { return Category; }
	FName GetAbilityName() const { return Name; }
	UTexture2D* GetIcon() const { return Icon; }
	FText GetDescription() const { return Description; }
	float GetCooldown() const { return Cooldown; }
};
