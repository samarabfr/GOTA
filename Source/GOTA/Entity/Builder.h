#pragma once

#include "Civilian.h"
#include "Builder.generated.h"

UCLASS()
class GOTA_API ABuilder : public ACivilian
{
	GENERATED_BODY()

private:
	bool IsConstructionSiteValid(const UBuilding* ConstructionSite) const;

protected:
	virtual void S_Work() override;
	virtual bool IsTileValidForWork(const ATile* Tile) const override;
};
