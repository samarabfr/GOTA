// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "GOTAAttributePopulation.h"
#include "Job.h"
#include "JobManager.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UJobManager : public UObject
{
	// Unreal Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	
	UJobManager();

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="JobManager")
	int32 AvailableWorkforce = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="JobManager")
	int32 CurrentlyEmployed = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="JobManager")
	TArray<UJob*> Jobs;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="JobManager")
	FJobIncome CurrentJobIncome;
	
	void BindToPopulationAttribute(UGOTAAttributePopulation* PopulationAttribute);
	void BindToBuilding(UBuilding* Building);
	void UnbindToBuilding(UBuilding* Building);

	UFUNCTION()
	void BuildingJobAdded(UJob* Job);
	UFUNCTION()
	void BuildingJobRemoved(UJob* Job);
	
	UFUNCTION()
	void WorkForceChanged(int32 Change);

	void DistributeWorkers();
};
