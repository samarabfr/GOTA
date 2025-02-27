#pragma once
#include "Building.h"

#include "Consumption.generated.h"

class UResourceStorage;

UCLASS(Blueprintable)
class GOTA_API UConsumption : public UObject
{
	GENERATED_BODY()
	// ------------------------------------ Replication Setup --------------------------------------


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------

private:
	UConsumption();

public:
	void S_Init(UBuilding* InBuilding);
	void S_Tick(float DeltaSeconds);
	void C_Tick(float DeltaSeconds);

	// ---------------------------------------- Utility ----------------------------------------

private:
	UPROPERTY()
	TWeakObjectPtr<UBuilding> Building;

	UPROPERTY(Replicated)
	UResourceStorage* ResourceStorage;

public:
	EConsumptionType GetConsumptionType() const;
	FGameResources GetCurrentResources() const;
	FGameResources GetResourceLimit() const;
	void S_AddResources(FGameResources Resources);
	bool ResourceStorageIsEmpty() const;
	
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceStorageOnIsEmptyChangedSignature, bool, IsEmpty);

	FOnResourceStorageIsEmptyChangedSignature FOnResourceStorageIsEmptyChanged;

	// ---------------------------------------- Consumption ----------------------------------------

public:
	float GetConsumptionPerSecond() const;
};
