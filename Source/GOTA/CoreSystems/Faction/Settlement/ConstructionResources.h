// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "ConstructionResources.generated.h"

USTRUCT(BlueprintType)
struct FConstructionResources
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Resources")
	float Food = 0;

	UPROPERTY(EditAnywhere, Category = "Resources")
	float Wood = 0;

	UPROPERTY(EditAnywhere, Category = "Resources")
	float Stone = 0;

	void Add(const float Amount, const EProductionType ProductionType);
	void Remove(const float Amount, const EProductionType ProductionType);

	void Add(const float Amount, const EResource Resource);
	void Remove(const float Amount, const EResource Resource);

	FConstructionResources& operator+=(const FConstructionResources& Other);
	FConstructionResources& operator-=(const FConstructionResources& Other);

	bool operator<(const FConstructionResources& Other) const;
	bool operator>(const FConstructionResources& Other) const;
	bool operator<=(const FConstructionResources& Other) const;
	bool operator>=(const FConstructionResources& Other) const;
	bool operator==(const FConstructionResources& Other) const;
	bool operator!=(const FConstructionResources& Other) const;

	FConstructionResources operator+(const FConstructionResources& Other) const;
	FConstructionResources operator-(const FConstructionResources& Other) const;

	static FConstructionResources Zero() { return FConstructionResources(0, 0, 0); }
};
