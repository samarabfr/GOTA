#pragma once

#include "Civilian.h"
#include "Hunter.generated.h"

UCLASS()
class GOTA_API AHunter : public ACivilian
{
	GENERATED_BODY()
	AHunter();
	
	virtual void ValidateStatus() override;
	virtual void Work() override;
	bool TryFindPath();
	bool IsTileValidForWork(const ATile* Tile) const;
};
