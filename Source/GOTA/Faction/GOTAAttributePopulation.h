// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTAAttributeLimited.h"
#include "GOTAAttributePopulation.generated.h"

// Define an enum for the religion
UENUM(BlueprintType)
enum class EReligion : uint8
{
	Guardian1 UMETA(DisplayName = "Guardian1"),
	Guardian2 UMETA(DisplayName = "Guardian2"),
	Guardian3 UMETA(DisplayName = "Guardian3"),
	Guardian4 UMETA(DisplayName = "Guardian4"),
	Colonists UMETA(DisplayName = "Colonists"),
	MAX UMETA(Hidden) // Sentinel value for enum size
};

UENUM(BlueprintType)
enum class EMood : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Fearful UMETA(DisplayName = "Fearful"),
	Aggressive UMETA(DisplayName = "Aggressive"),
	MAX UMETA(Hidden) // Sentinel value for enum size
};

UCLASS()
class GOTA_API UGOTAAttributePopulation : public UGOTAAttributeLimited
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UGOTAAttributePopulation();

protected:
	virtual void OnChange() override;
	
//====================================================================
//-------------------- Followers
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(Replicated)
	TArray<int32> Follower;
	void AddOneFollowerWeightedRandom(EReligion Exclude = EReligion::MAX);
	void SubtractOneFollowerWeightedRandom(EReligion Exclude = EReligion::MAX);
	void AddOneFollowerToGuardiansFullRandom();

public:
	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeFollower(EReligion Religion, int32 NumberOfFollower, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollower(EReligion Religion) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollowerNatives();

	UFUNCTION(BlueprintCallable, Category = "Population")
	void GetAllFollower(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4, int32& Colonists);

	//====================================================================
	//-------------------- Mood
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
};