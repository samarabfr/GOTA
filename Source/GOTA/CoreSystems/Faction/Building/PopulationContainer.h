// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Population.h"
#include "GOTA/CoreSystems/Utility/DelegateTypes.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "PopulationContainer.generated.h"

UCLASS(Blueprintable)
class GOTA_API UPopulationContainer : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UPopulationContainer();

	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnFPopulationChangedSig OnPopulationChanged;
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnGrowthChangedSig OnGrowthChanged;

	// ---------------------------------------------------------
	// Population Struct

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Population, BlueprintReadOnly, Category = "Population")
	FPopulation Population;

	UFUNCTION()
	void OnRep_Population(const FPopulation& OldPopulation);

	// ---------------------------------------------------------
	// Changing Population Values
	
	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangePopulationSize(int32 Change);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void IncreasePopulationSize(int32 Change);
	
	UFUNCTION(BlueprintCallable, Category = "Population")
	void DecreasePopulationSize(int32 Change);
	
	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeMaximum(int32 Change, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeFollower(ECultureLoyalty Religion, int32 Change, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeMood(EMood Mood, int32 Change, int32& Effective_Change);

private:
	void AddOneFollowerWeightedRandom(ECultureLoyalty Exclude = ECultureLoyalty::MAX);
	void SubtractOneFollowerWeightedRandom(ECultureLoyalty Exclude = ECultureLoyalty::MAX);
	void AddOneFollowerToGuardiansFullRandom();
	void SubtractOneMoodWeightedRandom(EMood Exclude = EMood::MAX);

	// ---------------------------------------------------------
	// Population Growth Stuff
public:
	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category = "Population")
	int32 Growth = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category = "Population")
	int32 GrowthChange = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category = "Population")
	int32 GrowthThreshold = 0;

	// ---------------------------------------------------------
	// Getters
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Population")
	FPopulation GetPopulation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollower(ECultureLoyalty Religion) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollowerNatives();

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllFollower(int32& Colonists, int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4);

	UFUNCTION(BlueprintCallable, Category = "Population")
	int32 GetMood(EMood Mood);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllMood(int32& Content, int32& Angry, int32& Fear);
};
