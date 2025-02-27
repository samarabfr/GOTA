#pragma once
#include "Building.h"

#include "Production.generated.h"

UCLASS(Blueprintable)
class GOTA_API UProduction : public UObject
{
	GENERATED_BODY()
	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------

public:
	void S_Init(UBuilding* InBuilding);

	// ---------------------------------------- Utility ----------------------------------------

private:
	UPROPERTY()
	TWeakObjectPtr<UBuilding> Building;

public:
	// returns what this building is producing, can be abstract things like "construction"
	EProductionType GetProductionType() const;

	// ---------------------------------------- Production ----------------------------------------

private:
	UPROPERTY(ReplicatedUsing=OnRep_ProductionPerSecond)
	float ProductionPerSecond = 0.0f;
	void RecalculateProductionPerSecond();

	UFUNCTION()
	void OnRep_ProductionPerSecond(float OldProductionPerSecond);

	UFUNCTION()
	void HandleEfficiencyChange(float EfficiencyChange);

public:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionPerSecondChangedSig, float, Change, float, NewValue);

	FOnProductionPerSecondChangedSig OnProductionPerSecondChanged;
	float GetProductionPerSecond() const;
};
