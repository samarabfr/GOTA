#pragma once
#include "GOTA/CoreSystems/Faction/Building/Population.h"

#include "TotalPopulation.generated.h"

UCLASS()
class GOTA_API UTotalPopulation : public UObject
{
	GENERATED_BODY()

	// ------------------Tracking Changes----------------
public:
	void RegisterPop(UPopulation* Pop);

	void UnregisterPop(UPopulation* Pop);

private:
	UFUNCTION()
	void UpdateSize(int16 ChangedBy);

	UFUNCTION()
	void UpdateAngry(int16 ChangedBy);

	UFUNCTION()
	void UpdateFear(int16 ChangedBy);

	// ------------------Variable Definition----------------
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 Size = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 Angry = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Population")
	int16 Fear = 0;

	// ---------------------Getters & Setter-------------------------
public:
	int16 GetSize() const { return Size; }

	int16 GetAngry() const { return Angry; }

	int16 GetFear() const { return Fear; }
};
