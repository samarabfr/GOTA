#pragma once

#include "Civilian.h"
#include "Forager.generated.h"

UCLASS()
class GOTA_API AForager : public ACivilian
{
	GENERATED_BODY()
	
	virtual void S_Work() override;
	virtual bool IsTileValidForWork(const ATile* Tile) const override;
	virtual bool S_TryFindPathToBestWorkTile() override;
};
