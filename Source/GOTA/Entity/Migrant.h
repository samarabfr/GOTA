#pragma once

#include "Civilian.h"
#include "Migrant.generated.h"

UCLASS()
class GOTA_API AMigrant : public ACivilian
{
	GENERATED_BODY()

	virtual void S_Work() override;
	virtual bool IsTileValidForWork(const ATile* Tile) const override;

	UPROPERTY(VisibleInstanceOnly)
	int32 Size = 0;

public:
	void SetSize(const int32 NewSize);
};
