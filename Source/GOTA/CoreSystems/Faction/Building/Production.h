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
	void S_Tick(float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

	// ---------------------------------------- Utility ----------------------------------------

private:
	UPROPERTY()
	TWeakObjectPtr<UBuilding> Building;

public:
	// returns what this building is producing, can be abstract things like "construction"
	EProductionType GetProductionType() const;

	// ---------------------------------------- Effective production ----------------------------------------

private:
	UFUNCTION()
	void HandleEfficiencyChange(float EfficiencyChange);
public:
	float GetEffectiveProductionPerSecond() const;
	// in seconds
	float GetEffectiveProductionTime() const;
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEffectiveProductionChangedSig);

	FOnEffectiveProductionChangedSig OnEffectiveProductionChanged;
	
	// ---------------------------------------- Progress ----------------------------------------
	
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float ProductionProgress = 0.0f;
	void S_ApplyProduction();

public:
	float GetProductionProgress() const;
};
