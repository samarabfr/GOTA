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

	// ------------------Variable Definition----------------------
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	EFaction Faction = EFaction::None;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Size, Category = "Population")
	int16 Size = 0;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_MaxSize, Category = "Population")
	int16 MaxSize = 0;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Angry, Category = "Population")
	int16 Angry = 0;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Fear, Category = "Population")
	int16 Fear = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	float GrowthProgress = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	float Growth = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	float GrowthThreshold = 0;

	// ---------------Changing Population Values-----------------------
	UFUNCTION()
	void OnRep_Size(const int16 OldValue);

	UFUNCTION()
	void OnRep_MaxSize(const int16 OldValue);

	UFUNCTION()
	void OnRep_Angry(const int16 OldValue);

	UFUNCTION()
	void OnRep_Fear(const int16 OldValue);

public:
	void SetFaction(EFaction NewFaction);

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
	void SubtractMoodWeightedRandom(const int16 Change);

	// ------------------Delegates----------------------
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInt16ChangedSig, int16, ChangedBy);

public:
	FOnInt16ChangedSig OnSizeChanged;
	FOnInt16ChangedSig OnMaxSizeChanged;
	FOnInt16ChangedSig OnAngryChanged;
	FOnInt16ChangedSig OnFearChanged;

	// --------------------Getters and Setters----------------------
public:
	EFaction GetFaction() const { return Faction; }

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
};
