// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementBalance.h"
#include "GOTA/CoreSystems/Faction/Building/PopulationContainer.h"
#include "PopulationSummary.generated.h"

UCLASS()
class GOTA_API UPopulationSummary : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFPopulationChangedSig, FPopulation, ChangedBy);

	UPROPERTY()
	TArray<UPopulationContainer*> PopCons;
	ECultureLoyalty DefaultCulture = ECultureLoyalty::Colonists;

public:
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnFPopulationChangedSig OnPopulationChanged;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Population, BlueprintReadOnly, Category = "Population")
	FPopulation Population;

	UFUNCTION()
	void OnRep_Population(const FPopulation& OldPopulation);

	FPopulation ExtractArmyPopulation(USettlementBalance* Balance);

	// ---------------------------------------------------------
	// Keeping Track of Population Changes

	UFUNCTION(BlueprintCallable)
	void RegisterPopulationContainer(UPopulationContainer* PopulationContainer);

	UFUNCTION(BlueprintCallable)
	void RegisterPopulationSummary(UPopulationSummary* PopulationSummary);

	UFUNCTION(BlueprintCallable)
	void UnregisterPopulationContainer(UPopulationContainer* PopulationContainer);

	UFUNCTION(BlueprintCallable)
	void UnregisterPopulationSummary(UPopulationSummary* PopulationSummary);

	UFUNCTION()
	void UpdatePopulation(FPopulation Change);

	void SetDefaultCulture(ECultureLoyalty Culture);

	// ---------------------------------------------------------
	// Getters

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollower(ECultureLoyalty Culture) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetNativeFollowers();

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4, int32& Colonists);

	UFUNCTION(BlueprintCallable, Category = "Population")
	int32 GetMood(EMood Mood);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllMood(int32& Content, int32& Angry, int32& Fear);
};
