// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "GameResources.generated.h"

USTRUCT(BlueprintType)
struct FGameResources
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Resources")
	float Food = 0;

	UPROPERTY(EditAnywhere, Category = "Resources")
	float Wood = 0;

	UPROPERTY(EditAnywhere, Category = "Resources")
	float Stone = 0;

	void AddProduction(const float Amount, const EProductionType ProductionType);
	void RemoveProduction(const float Amount,const  EProductionType ProductionType);

	void AddConsumption(const float Amount,const  EConsumptionType ConsumptionType);
	void RemoveConsumption(const float Amount,const  EConsumptionType ConsumptionType);

	FGameResources& operator+=(const FGameResources& Other);
	FGameResources& operator-=(const FGameResources& Other);

	bool operator<(const FGameResources& Other) const;
	bool operator>(const FGameResources& Other) const;
	bool operator<=(const FGameResources& Other) const;
	bool operator>=(const FGameResources& Other) const;

	FGameResources operator+(const FGameResources& Other) const;
	FGameResources operator-(const FGameResources& Other) const;
};
