#pragma once

#include "GameplayTagContainer.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
#include "GOTA/CoreSystems/Tile/GameplayTagRule.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "BuildingSettings.generated.h"

class AArmy;
class ACivilian;

UCLASS()
class GOTA_API UBuildingSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Building")
	FName Name = "Unnamed";

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FText Description;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	TArray<FGameplayTagRule> PlacementRules;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	FGameResources Cost;

	UPROPERTY(EditDefaultsOnly, Category="Building")
	int32 Housing = 0;

	//------------------Income------------------

	UPROPERTY(EditDefaultsOnly, Category="Income")
	bool bIncomeEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Income")
	EProductionType IncomeType = EProductionType::None;

	UPROPERTY(EditDefaultsOnly, Category="Income")
	float IncomeTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Income")
	float IncomeAmount = 0.0f;

	//--------------------------Civilian Entity-------------------

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	bool bCivilianEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	TSubclassOf<ACivilian> CivilianClass;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	float SecondsPerWorkCycle = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	int32 WorkAmountPerCycle = 0;

	UPROPERTY(EditDefaultsOnly, Category="Civilian")
	float CivilianSecondsPerMove = 0.0f;

	//--------------------------Army-------------------

	UPROPERTY(EditDefaultsOnly, Category="Army")
	bool bArmyEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float SecondsPerRecruitCycle = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float ArmySecondsPerMove = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Army")
	float ArmyRespawnTime = 0.0f;
};
