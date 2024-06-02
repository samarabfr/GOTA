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
	MAX UMETA(Hidden) // Sentinel value for array size
};

UCLASS()
class GOTA_API UGOTAAttributePopulation : public UGOTAAttributeLimited
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UGOTAAttributePopulation();

private:
	UPROPERTY(Replicated)
	TArray<int32> Follower;
	void AddOneFollowerWeightedRandom(EReligion Exclude);
	void SubtractOneFollowerWeightedRandom(EReligion Exclude);
	void AddOneFollowerToGuardiansFullRandom();

protected:
	virtual void OnChange() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Population")
	void ChangeFollower(EReligion Religion, int32 NumberOfFollower, int32& Effective_Change);

	UFUNCTION(BlueprintCallable, Category = "Population")
	int32 GetFollower(EReligion Religion) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	int32 GetFollowerNatives();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population")
	void GetAllFollowers(int32& Guardian1, int32& Guardian2, int32& Guardian3, int32& Guardian4, int32& Colonists);
};
