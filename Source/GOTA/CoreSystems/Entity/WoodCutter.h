#pragma once

#include "Civilian.h"
#include "Woodcutter.generated.h"

UCLASS()
class GOTA_API AWoodcutter : public ACivilian
{
	GENERATED_BODY()
	AWoodcutter();
	
	virtual void S_Work() override;
	virtual bool IsTileValidForWork(ATile* Tile) const override;
};
