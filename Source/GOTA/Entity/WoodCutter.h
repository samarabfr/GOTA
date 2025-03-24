#pragma once

#include "Civilian.h"
#include "Woodcutter.generated.h"

UCLASS()
class GOTA_API AWoodcutter : public ACivilian
{
	GENERATED_BODY()
	
	virtual void S_Work() override;
	virtual bool IsTileValidForWork(const ATile* Tile) const override;
	virtual TArray<ATile*> FindPathToBestWorkTile() const override;
};
