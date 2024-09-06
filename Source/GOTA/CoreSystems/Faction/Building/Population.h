// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "Population.generated.h"

UCLASS(Blueprintable)
class GOTA_API UPopulation : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UPopulation();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedSig, UPopulation*, New);

public:
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnChangedSig OnChanged;

	// ------------------Population Values----------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	int16 Size = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	int16 MaxSize = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	int16 Angry = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	int16 Fear = 0;

	// ---------------Changing Population Values-----------------------
public:
	void ChangeSize(const int16 Change);

	void IncreaseSize(const int16 Change);

	void DecreaseSize(const int16 Change);

	void ChangeMaxSize(const int16 Change);

	void IncreaseMaxSize(const int16 Change);

	void DecreaseMaxSize(const int16 Change);

	void ChangeMood(const EMood Mood, const int16 Change);

	void IncreaseMood(const EMood Mood, const int16 Change);

	void DecreaseMood(const EMood Mood, const int16 Change);

	void IncreaseAngry(const int16 Change);

	void DecreaseAngry(const int16 Change);

	void IncreaseFear(const int16 Change);

	void DecreaseFear(const int16 Change);

private:
	void SubtractOneMoodWeightedRandom();
	void Changed();

	// ---------------------------Growth----------------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	float GrowthProgress = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	float Growth = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	float GrowthThreshold = 0;

	// --------------------Getters and Setters----------------------
public:
	int16 GetSize() const { return Size; }
	
	int16 GetMaxSize() const { return MaxSize; }

	int16 GetAngry() const { return Angry; }

	int16 GetFear() const { return Fear; }

	float GetGrowthProgress() const { return GrowthProgress; }

	float GetGrowth() const { return Growth; }

	float GetGrowthThreshold() const { return GrowthThreshold; }

	int16 GetContentMood() const;

	int16 GetMood(const EMood Mood) const;

	void GetAllMood(int16& Content_, int16& Angry_, int16& Fear_) const;

	// --------------------Operators-----------------------

	UPopulation& operator+=(const UPopulation& Other);

	UPopulation& operator-=(const UPopulation& Other);
};
