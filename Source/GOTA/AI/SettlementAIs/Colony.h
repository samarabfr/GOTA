#pragma once

#include "NewBuildingImportanceRatings.h"
#include "GOTA/Settlement/Settlement.h"

#include "Colony.generated.h"


class AArmy;
class UColonyBrainSettings;
class UBuildingSettings;

UCLASS()
class AColony : public ASettlement
{
	GENERATED_BODY()
	// ----------------------- LifeCycle -----------------------
public:
	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

private:
	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaSeconds) override;

	// --------------------Building----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> PossibleBuildings;

	void FigureOutBuilding();

	bool ShouldBuild() const;

	ATile* FindBuildableTile() const;

	UBuildingSettings* SelectNewBuilding() const;

	static float CalculateScore(const UBuildingSettings* Data, FNewBuildingImportanceRatings ImportanceRatings);

	FNewBuildingImportanceRatings CalculateImportanceRatings() const;

	void CalculateScores(FNewBuildingImportanceRatings ImportanceRatings);

	// --------------------Army----------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	float SendArmiesIntervalTimeLeft = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float SendArmiesIntervalTime = 60.0f;

	// --------------------Importances----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	float FoodImportance = 1;

	UPROPERTY(EditDefaultsOnly)
	float FoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly)
	float WoodImportance = 1;

	UPROPERTY(EditDefaultsOnly)
	float WoodImportanceDescent = 0.1;

	UPROPERTY(EditDefaultsOnly)
	float StoneImportance = 1;

	UPROPERTY(EditDefaultsOnly)
	float StoneImportanceDescent = 0.1;
};
