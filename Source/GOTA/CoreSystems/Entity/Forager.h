#pragma once

#include "Civilian.h"
#include "Forager.generated.h"

UCLASS()
class GOTA_API AForager : public ACivilian
{
	GENERATED_BODY()
	AForager();
	
	virtual void ValidateStatus() override;
	virtual void Work() override;
	bool TryFindPath();
	bool IsTileValidForWork(const ATile* Tile) const;
};
