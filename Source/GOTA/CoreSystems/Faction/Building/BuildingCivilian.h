#pragma once
#include "Building.h"

#include "BuildingCivilian.generated.h"

UCLASS(Blueprintable)
class GOTA_API UBuildingCivilian : public UBuilding
{
	GENERATED_BODY()
	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	virtual void S_Tick(float DeltaSeconds) override;
	virtual void C_Tick(const float DeltaSeconds) override;
protected:
	virtual void BeginDestroy() override;

	
	// ---------------- Civilian Entity ----------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	ACivilian* Civilian;

	void SetCivilian(ACivilian* NewCivilian);

	virtual void FinishConstruction() override;

public:
	ACivilian* GetCivilian() const { return Civilian; }
};