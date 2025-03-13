#pragma once

#include "SettlementAIController.h"

#include "ColonyAIController.generated.h"


class ATile;
class AArmy;
class UBuildingSettings;

UCLASS()
class AColonyAIController : public ASettlementAIController
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
private:
	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	AColonyAIController();

public:
	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

	// --------------------Building----------------------
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<UBuildingSettings*> PossibleBuildings;

	void FigureOutBuilding();

	bool ShouldBuild() const;

	ATile* FindBuildableTile() const;

	UBuildingSettings* SelectNewBuilding() const;

	// --------------------Army----------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	float SendArmiesIntervalTimeLeft = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float SendArmiesIntervalTime = 60.0f;
};
