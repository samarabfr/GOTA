// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Population.h"
#include "PopulationSummary.generated.h"

/**
 * 
 */
UCLASS()
class GOTA_API UPopulationSummary : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UPopulationSummary();

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

public:
	UPROPERTY(ReplicatedUsing=OnRep_Current, BlueprintReadOnly, Category = "Population")
	int32 Current = 0;

	UPROPERTY(ReplicatedUsing=OnRep_Maximum, BlueprintReadOnly, Category = "Population")
	int32 Maximum = 0;

	UPROPERTY(ReplicatedUsing=OnRep_Follower)
	TArray<int32> Follower;

	UPROPERTY(ReplicatedUsing=OnRep_Moods)
	TArray<int32> Moods;

	UFUNCTION()
	void OnRep_Current();

	UFUNCTION()
	void OnRep_Maximum();

	UFUNCTION()
	void OnRep_Follower();
	
	UFUNCTION()
	void OnRep_Moods();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollower(EReligion Religion) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollowerNatives();

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4, int32& Colonists);

	UFUNCTION(BlueprintCallable, Category = "Population")
	int32 GetMood(EMood Mood);

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllMood(int32& Neutral, int32& Fearful, int32& Aggressive);

	UFUNCTION(BlueprintCallable)
	void RegisterPopulation(UPopulation* Population);

	UFUNCTION(BlueprintCallable)
	void RegisterPopulationSummary(UPopulationSummary* PopulationSummary);

	UFUNCTION()
	void UpdatePopulation(int32 Change);

	UFUNCTION()
	void UpdateMaximum(int32 Change);

	UFUNCTION()
	void UpdateFollower(int32 ChangeC, int32 ChangeG1, int32 ChangeG2, int32 ChangeG3, int32 ChangeG4);

	UFUNCTION()
	void UpdateMood(int32 NeutralChange, int32 FearfulChange, int32 AggressiveChange);
};
