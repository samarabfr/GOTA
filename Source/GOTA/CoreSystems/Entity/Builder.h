#pragma once

#include "Civilian.h"
#include "Builder.generated.h"

UCLASS()
class GOTA_API ABuilder : public ACivilian
{
	GENERATED_BODY()
	ABuilder();
	
	virtual void S_Work() override;
	virtual bool IsTileValidForWork(ATile* Tile) const override;
};
