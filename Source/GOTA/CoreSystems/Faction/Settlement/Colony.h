#pragma once
#include "Settlement.h"
#include "NewBuildingImportanceRatings.h"

#include "Colony.generated.h"


class UColonyBrainSettings;
class UBuildingSettings;

UCLASS()
class AColony : public ASettlement
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	UColonyBrainSettings* ColonyBrainSettings;

	void InitColonyBrainSettings();
	
	// --------------------Building----------------------
private:
	void FigureOutBuilding();

	bool ShouldBuild() const;

	ATile* FindBuildableTile() const;

	UBuildingSettings* SelectNewBuilding() const;

	static float CalculateScore(const UBuildingSettings* Data, FNewBuildingImportanceRatings ImportanceRatings);

	FNewBuildingImportanceRatings CalculateImportanceRatings() const;

	void CalculateScores(FNewBuildingImportanceRatings ImportanceRatings);
};
