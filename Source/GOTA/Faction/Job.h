// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JobDataAsset.h"
#include "JobIncome.h"
#include "Job.generated.h"

UCLASS(Blueprintable)
class GOTA_API UJob : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UJob();

public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category="Job")
	bool IsWorked;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Job")
	int32 Tier;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Job")
	UJobDataAsset* DataAsset;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Job")
	void Setup(UJobDataAsset* InDataAsset, int32 InTier);

	UFUNCTION(BlueprintCallable, Category = "Job")
	FJobIncome GetJobIncome();
};
