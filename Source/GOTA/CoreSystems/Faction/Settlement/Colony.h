#pragma once
#include "Settlement.h"
#include "NewBuildingImportanceRatings.h"

#include "Colony.generated.h"


UCLASS()
class AColony : public ASettlement
{
	GENERATED_BODY()
	// ----------------------- LifeCycle -----------------------
protected:
	AColony();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

private:
	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaSeconds) override;

	// --------------------Building----------------------
private:
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

	void SendArmies();
	TArray<AArmy*> GetAllColonyArmies();
};
