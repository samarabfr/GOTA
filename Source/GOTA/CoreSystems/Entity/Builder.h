#pragma once

#include "Civilian.h"
#include "Builder.generated.h"

UCLASS()
class GOTA_API ABuilder : public ACivilian
{
	GENERATED_BODY()
	ABuilder();
	
	virtual void ValidateStatus() override;
	virtual void Work() override;
	bool TryFindPath();
	bool IsTileValidForWork(const ATile* Tile) const;
};
