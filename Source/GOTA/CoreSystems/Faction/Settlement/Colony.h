#pragma once
#include "Settlement.h"
#include "NewBuildingImportanceRatings.h"

#include "Colony.generated.h"


UCLASS()
class AColony : public ASettlement
{
	GENERATED_BODY()
	AColony();

	virtual void Tick(float DeltaSeconds) override;

	// --------------------Building----------------------
private:
	void FigureOutBuilding();

	bool ShouldBuild() const;

	ATile* FindBuildableTile() const;

	UBuildingDataAsset* SelectNewBuilding() const;

	static float CalculateScore(const UBuildingDataAsset* Data, FNewBuildingImportanceRatings ImportanceRatings);

	FNewBuildingImportanceRatings CalculateImportanceRatings() const;

	void CalculateScores(FNewBuildingImportanceRatings ImportanceRatings);
};
