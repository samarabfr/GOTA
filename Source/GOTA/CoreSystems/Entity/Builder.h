#pragma once

#include "Civilian.h"
#include "Builder.generated.h"

UCLASS()
class GOTA_API ABuilder : public ACivilian
{
	GENERATED_BODY()
	ABuilder();
	
	virtual void S_Work() override;
	virtual bool IsTileValidForWork(const ATile* Tile) const override;
	TArray<ATile*> FindBestWorkTiles();
	virtual bool TryFindPathToBestWorkTile() override;
	virtual bool IsCurrentTileAmongBestWorkTiles() override;
};
