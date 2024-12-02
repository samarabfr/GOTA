#pragma once
#include "Building.h"

#include "BuildingDirectProduction.generated.h"

UCLASS(Blueprintable)
class GOTA_API UBuildingDirectProduction : public UBuilding
{
	GENERATED_BODY()
	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	virtual void S_Tick(float DeltaSeconds) override;
	virtual void C_Tick(const float DeltaSeconds) override;
protected:
	virtual void BeginDestroy() override;

	// ---------------------------------------- Direct production ----------------------------------------
	// Direct Production will be produced every [DirectProductionTime]/[Efficiency] seconds.
	// The Amount is always the same.
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float DirectProductionProgress = 0.0f;

	void S_ApplyDirectProduction();
	
public:
	float GetDirectProductionProgress() const { return DirectProductionProgress; }
};
