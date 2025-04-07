#pragma once

#include "SettlementAIController.h"
#include "GOTA/Settlement/ConstructionResources.h"

#include "ColonyAI_T1.generated.h"

class ATile;
class AArmy;
class UBuildingSettings;

UCLASS()
class AColonyAI_T1 : public ASettlementAIController
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
private:
	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	AColonyAI_T1();

public:
	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);
	virtual void Possess(ASettlement* Settlement) override;

	// --------------------Building----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	float FoodBuildingFoodResourceThreshold = 300.0f;

	UPROPERTY(EditDefaultsOnly)
	FConstructionResources BarracksIncomeThreshold = FConstructionResources(3, 2, 0.5);

	UPROPERTY(EditDefaultsOnly)
	int32 BarracksUnprotectedBuildingsThreshold = 5;

	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> PossibleBuildings;

	void FigureOutBuilding();
	bool ShouldBuild() const;
	ATile* FindBuildableTile() const;
	TArray<ATile*> FindTilesWithMostNeighborPop() const;
	UBuildingSettings* SelectNewBuilding() const;


	// -------------------- Logging ----------------------
private:
	TMap<FName, int32> BuildingsCounter;

public:
	virtual TSharedPtr<FJsonObject> Log() override;
};
