// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Building/Building.h"
#include "BuildingSummary.generated.h"

UCLASS()
class GOTA_API UBuildingSummary : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UBuildingSummary();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnythingChangedSignature);
	
public:
	UPROPERTY(BlueprintAssignable, Category="Building")
	FOnAnythingChangedSignature OnChanged;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Building")
	TMap<EProductionType, int32> ProductionMap;
	
	void RegisterBuildingProduction(UBuilding* Building);
	
	void UnregisterBuildingProduction(UBuilding* Building);

	UFUNCTION()
	void UpdateBuildingProduction(int32 Change, EProductionType Type);
};