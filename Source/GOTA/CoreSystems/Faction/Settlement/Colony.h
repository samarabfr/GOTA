#pragma once

#include "CoreMinimal.h"
#include "BuildingProject.h"
#include "BuildingProjectScore.h"
#include "Settlement.h"
#include "SettlementImportanceRatings.h"
#include "Colony.generated.h"

UCLASS()
class AColony : public ASettlement
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	AColony();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// --------------------Building project----------------------
public:
	UPROPERTY()
	TArray<UBuildingDataAsset*> PossibleBuildings;
	
	UPROPERTY()
	TArray<UBuildingProject*> BuildingProjectPool;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category="Settlement")
	UBuildingProject* CurrentBuildingProject;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	FSettlementImportanceRatings ImportanceRatings;

	UPROPERTY(VisibleInstanceOnly, Category="Settlement")
	TArray<FBuildingProjectScore> Scores;
	
private:
	void SetCurrentBuildingProject(UBuildingProject* NewCurrentBuildingProject);

	void FigureOutBuilding();

	void SelectNewBuildingProject();

	void FillBuildingPool();

	void CalculateImportances();
};
