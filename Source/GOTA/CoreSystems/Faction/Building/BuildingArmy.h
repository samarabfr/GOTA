#pragma once
#include "Building.h"

#include "BuildingArmy.generated.h"

UCLASS(Blueprintable)
class GOTA_API UBuildingArmy : public UBuilding
{
	GENERATED_BODY()
	// ------------------------------------ Replication Setup --------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// ---------------------------------------- Lifecycle ----------------------------------------
public:
	virtual void S_Tick(float DeltaSeconds) override;
	virtual void C_Tick(const float DeltaSeconds) override;

	virtual void PrepareDelete() override;

	// ---------------- Army ----------------
private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	AArmy* Army;

	UPROPERTY(VisibleInstanceOnly)
	float ArmyRespawnTimer = 0.0F;

	void SetArmy(AArmy* NewArmy);

public:
	virtual AArmy* GetArmy() const override { return Army; }
};
