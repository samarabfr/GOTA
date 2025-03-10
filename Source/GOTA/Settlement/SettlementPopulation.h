// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementPopulation.generated.h"

class UPopulation;
class AGS_Ingame;

UCLASS()
class GOTA_API USettlementPopulation : public UObject
{
	GENERATED_BODY()

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedSig, USettlementPopulation*, New);

	UPROPERTY()
	TArray<UPopulation*> Populations;

	// ------------------Tracking Changes----------------
public:
	void RegisterPop(UPopulation* Pop);

	void UnregisterPop(UPopulation* Pop);

	void StarveRandomPop();

private:
	UFUNCTION()
	void UpdateSize(int16 ChangedBy);

	UFUNCTION()
	void UpdateMaxSize(int16 ChangedBy);

	UFUNCTION()
	void UpdateAngry(int16 ChangedBy);

	UFUNCTION()
	void UpdateFear(int16 ChangedBy);

	// ------------------Variable Definition----------------
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 Size = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 MaxSize = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 Angry = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 Fear = 0;

	// ---------------------Getters & Setter-------------------------
public:
	int16 GetSize() const { return Size; }

	int16 GetMaxSize() const { return MaxSize; }

	int16 GetAngry() const { return Angry; }

	int16 GetFear() const { return Fear; }

	void S_SetStarving(bool IsStarving);
	void S_SetGrowthPerOwnPop(float NewGrowthPerOwnPop);
	void S_SetGrowthPerNeighborPop(float NewGrowthPerNeighborPop);
};
