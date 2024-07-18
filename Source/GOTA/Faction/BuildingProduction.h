// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Population.h"
#include "BuildingProduction.generated.h"

UENUM(BlueprintType)
enum class EProductionType : uint8
{
	Foraging UMETA(DisplayName = "Foraging"),
	Woodcutting UMETA(DisplayName = "Woodcutting"),
	Hunting UMETA(DisplayName = "Hunting"),
	Converting UMETA(DisplayName = "Converting"),
	Expansion UMETA(DisplayName = "Expansion"),
	MAX UMETA(Hidden) // Sentinel value for enum size
};

UCLASS()
class GOTA_API UBuildingProduction : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnythingChangedSignature);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionChangedSignature,
	                                             int32, Changed,
	                                             EProductionType, Type);

	int32 CurrentPopulation = 0;

	void RecalculateProduction();

public:
	FOnAnythingChangedSignature OnChanged;
	FOnProductionChangedSignature OnProductionChanged;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Building")
	int32 PopulationThreshold = 0;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Building")
	int32 ProductionPerThreshold = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building")
	int32 Production = 0;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Building")
	EProductionType ProductionType = EProductionType::MAX;

	UFUNCTION(BlueprintCallable)
	void BindToPopulation(UPopulation* Population);

	UFUNCTION()
	void UpdatePopulation(int32 Change);
};
