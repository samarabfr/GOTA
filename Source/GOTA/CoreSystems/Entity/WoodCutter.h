#pragma once

#include "Civilian.h"
#include "Woodcutter.generated.h"

UCLASS()
class GOTA_API AWoodcutter : public ACivilian
{
	GENERATED_BODY()
	AWoodcutter();
	
	virtual void S_Work() override;
	virtual bool IsTileValidForWork(const ATile* Tile) const override;
	virtual bool S_TryFindPathToBestWorkTile() override;
};
