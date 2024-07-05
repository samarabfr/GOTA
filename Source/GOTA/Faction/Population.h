// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopulationValues.h"
#include "Population.generated.h"

UCLASS()
class GOTA_API UPopulation : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UPopulation();

	// Delegate
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnythingChangedSignature);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedSignature, int32, ChangedBy);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMoodChangedSignature,
	                                               int32, NeutralChange,
	                                               int32, FearfulChange,
	                                               int32, AggressiveChange);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnFollowerChangedSignature,
	                                              int32, CFollowerChange,
	                                              int32, G1FollowerChange,
	                                              int32, G2FollowerChange,
	                                              int32, G3FollowerChange,
	                                              int32, G4FollowerChange);

public:
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnAnythingChangedSignature OnChanged;
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnChangedSignature OnPopulationChanged;
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnChangedSignature OnMaximumChanged;
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnFollowerChangedSignature OnFollowerChanged;
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnMoodChangedSignature OnMoodChanged;
	UPROPERTY(BlueprintAssignable, Category="Population")
	FOnChangedSignature OnWorkforceChanged;

public:
	UPROPERTY(BlueprintGetter=GetCurrent, Category = "Population")
	int32 Current = 0;

	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Population")
	int32 GetCurrent() const;

	UPROPERTY(BlueprintGetter=GetCurrent, Category = "Population")
	int32 Maximum = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangePopulation(int32 Change, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeMaximum(int32 Change, int32& Effective_Change);

	//====================================================================
	//                           Followers
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(Replicated)
	TArray<int32> Follower;
	void AddOneFollowerWeightedRandom(EReligion Exclude = EReligion::MAX);
	void SubtractOneFollowerWeightedRandom(EReligion Exclude = EReligion::MAX);
	void AddOneFollowerToGuardiansFullRandom();

public:
	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeFollower(EReligion Religion, int32 Change, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollower(EReligion Religion) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollowerNatives();

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4, int32& Colonists);

	//====================================================================
	//                            Mood
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(Replicated)
	TArray<int32> Moods;
	void SubtractOneMoodWeightedRandom(EMood Exclude = EMood::MAX);

public:
	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeMood(EMood Mood, int32 Change, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, Category = "Population")
	int32 GetMood(EMood Mood);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllMood(int32& Neutral, int32& Fearful, int32& Aggressive);

	//====================================================================
	//                          Workforce
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 PopToWorkforceRatio = 4;

	UPROPERTY(BlueprintGetter=GetWorkforce, Replicated)
	int32 Workforce = 0;

	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Population")
	int32 GetWorkforce();

	void CalculateWorkforce();
};
