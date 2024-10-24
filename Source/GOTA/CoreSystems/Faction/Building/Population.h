// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "Population.generated.h"

class UPopulationSettings;
class USettlementSettings;

UCLASS(Blueprintable)
class GOTA_API UPopulation : public UObject
{
	GENERATED_BODY()

	// ------------------- Replication Setup -------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override;

	// ------------------- LifeCycle -------------------
public:
	void S_Init(UPopulationSettings* InSettings);

	void S_Tick(const float DeltaSeconds);

	void C_Tick(const float DeltaSeconds);

	// ------------------- Utility -------------------
private:
	UPROPERTY(Replicated)
	UPopulationSettings* Settings;
	
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInt16ChangedSig, int16, ChangedBy);

	// ------------------- Size -------------------
private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Size, Category = "Population")
	int16 Size = 0;

	UFUNCTION()
	void OnRep_Size(const int16 OldValue);

public:
	int16 GetSize() const { return Size; }
	
	FOnInt16ChangedSig OnSizeChanged;
	
	void S_ChangeSize(const int16 Change);

	void S_IncreaseSize(const int16 Change);

	void S_DecreaseSize(const int16 Change);

	// ------------------- MaxSize -------------------
private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_MaxSize, Category = "Population")
	int16 MaxSize = 0;
	
	UFUNCTION()
	void OnRep_MaxSize(const int16 OldValue);

public:
	int16 GetMaxSize() const { return MaxSize; }
	
	FOnInt16ChangedSig OnMaxSizeChanged;
	
	void S_ChangeMaxSize(const int16 Change);

	void S_IncreaseMaxSize(const int16 Change);

	void S_DecreaseMaxSize(const int16 Change);
	
	// ------------------- Growth -------------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Population")
	float GrowthProgress = 0.0f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 NeighborSize = 0;
	
	void ApplyGrowth(const float DeltaSeconds);

public:
	float GetGrowth() const;
	
	void NeighborChangedPopSize(int16 Amount);

	float GetGrowthProgress() const { return GrowthProgress; }
	
	// ------------------- Mood -------------------
private:
	void S_SubtractMoodWeightedRandom(const int16 Change);
	
public:
	int16 GetContentMood() const;

	int16 GetMood(const EMood Mood) const;

	void GetAllMood(int16& Content_, int16& Angry_, int16& Fear_) const;
	
	void S_ChangeMood(const EMood Mood, const int16 Change);

	void S_IncreaseMood(const EMood Mood, const int16 Change);

	void S_DecreaseMood(const EMood Mood, const int16 Change);
	
	// ------------------- Angry Mood -------------------
private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Angry, Category = "Population")
	int16 Angry = 0;
	
	UFUNCTION()
	void OnRep_Angry(const int16 OldValue);

public:
	int16 GetAngry() const { return Angry; }
	
	FOnInt16ChangedSig OnAngryChanged;

	void S_IncreaseAngry(const int16 Change);

	void S_DecreaseAngry(const int16 Change);

	// ------------------- Fear Mood -------------------
private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Fear, Category = "Population")
	int16 Fear = 0;

	UFUNCTION()
	void OnRep_Fear(const int16 OldValue);
	
	void FearChanged(const int16 Change);
	
public:
	int16 GetFear() const { return Fear; }
	
	FOnInt16ChangedSig OnFearChanged;

	void IncreaseFear(const int16 Change);

	void DecreaseFear(const int16 Change);
};
