// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementBalance.h"
#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "PopulationSummary.generated.h"

UCLASS()
class GOTA_API UPopulationSummary : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedSig, UPopulationSummary*, New);

	UPROPERTY()
	TArray<UPopulation*> PopCons;

public:
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnChangedSig OnChanged;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int32 Size = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int32 Angry = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int32 Fear = 0;

	// ---------------------------------------------------------
	// Keeping Track of Population Changes

	UFUNCTION(BlueprintCallable)
	void RegisterPopulationContainer(UPopulation* PopulationContainer);

	UFUNCTION(BlueprintCallable)
	void RegisterPopulationSummary(UPopulationSummary* PopulationSummary);

	UFUNCTION(BlueprintCallable)
	void UnregisterPopulationContainer(UPopulation* PopulationContainer);

	UFUNCTION(BlueprintCallable)
	void UnregisterPopulationSummary(UPopulationSummary* PopulationSummary);

	UFUNCTION()
	void UpdatePopulation(FPopulation Change);

	// ---------------------------------------------------------
	// Getters

	UFUNCTION(BlueprintCallable, Category = "Population")
	int32 GetMood(EMood Mood);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllMood(int32& Content, int32& Angry, int32& Fear);
};
