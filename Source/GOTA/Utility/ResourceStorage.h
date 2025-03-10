// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "ResourceStorage.generated.h"

/**
 * A general storage for FGameResources
 */
UCLASS()
class GOTA_API UResourceStorage : public UObject
{
	GENERATED_BODY()

	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ------------------------------------ Lifecycle --------------------------------------

public:
	void S_Init(float InLimit, bool ReplicateOnAdding = true, bool ReplicateOnRemoving = true);

	// ------------------------------------ Utility --------------------------------------

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bReplicateOnAdding = true;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bReplicateOnRemoving = true;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Limit = 0.0f;

public:
	void S_SetReplicateOnAdding(bool ReplicateOnAdding);
	void S_SetReplicateOnRemoving(bool ReplicateOnRemoving);
	void S_SetLimit(float NewLimit);
	float GetLimit() const;

	// ------------------------------------ Current --------------------------------------

private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Current)
	float Current = 0.0f;

	UFUNCTION()
	void OnRep_Current();

	// broadcasts events but doesn't replicate 
	void SetCurrent(float NewCurrent);

public:
	float GetCurrent() const;
	bool IsEmpty() const;
	bool IsFull() const;
	// doesn't replicate
	void Add(float Amount);
	void Remove(float Amount);
	void Empty();
	void Fill();
	// Replicates
	void S_Add(float Amount);
	void S_Remove(float Amount);
	void S_Empty();
	void S_Fill();

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentChangedSignature, float, NewCurrent);

	FOnCurrentChangedSignature OnCurrentChanged;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsEmptyChangedSignature, bool, IsEmpty);

	FOnIsEmptyChangedSignature OnIsEmptyChanged;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsFullChangedSignature, bool, IsFull);

	FOnIsFullChangedSignature OnIsFullChanged;
};
