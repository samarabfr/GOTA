// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTA/CoreSystems/Faction/Settlement/GameResources.h"
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
	void S_Init(FGameResources ResourceLimit, bool ReplicateOnAdding = true, bool ReplicateOnRemoving = true);

	// ------------------------------------ Utility --------------------------------------

private:
	UPROPERTY(Replicated)
	bool bReplicateOnAdding = true;

	UPROPERTY(Replicated)
	bool bReplicateOnRemoving = true;

	UPROPERTY(Replicated)
	FGameResources ResourceLimit = FGameResources();

public:
	void S_SetReplicateOnAdding(bool ReplicateOnAdding);
	void S_SetReplicateOnRemoving(bool ReplicateOnRemoving);
	void S_SetResourceLimit(FGameResources NewResourceLimit);
	FGameResources GetResourceLimit() const;
	
	// ------------------------------------ Current Resources --------------------------------------
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentResources)
	FGameResources CurrentResources = FGameResources();

	UFUNCTION()
	void OnRep_CurrentResources();

	// doesnt replicate nor broadcasts any events
	void AddResources(FGameResources Resources);
	void RemoveResources(FGameResources Resources);

public:
	FGameResources GetCurrentResources() const;
	bool IsEmpty();
	bool IsFull();
	// Replicates and broadcasts events
	void S_AddResources(FGameResources Resources);
	void S_RemoveResources(FGameResources Resources);
	// broadcasts events but doesnt replicate
	void C_AddResources(FGameResources Resources);
	void C_RemoveResources(FGameResources Resources);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentResourcesChangedSignature, FGameResources,
	                                            NewCurrentResources);

	FOnCurrentResourcesChangedSignature OnCurrentResourcesChanged;
	
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsEmptyChangedSignature, bool, IsEmpty);

	FOnIsEmptyChangedSignature OnIsEmptyChanged;
	
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsFullChangedSignature, bool, IsFull);

	FOnIsFullChangedSignature OnIsFullChanged;
};
