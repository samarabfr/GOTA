#pragma once

#include "Civilian.h"
#include "Migrant.generated.h"

UCLASS()
class GOTA_API AMigrant : public ACivilian
{
	GENERATED_BODY()
	AMigrant();
	
	virtual void ValidateStatus() override;
	virtual void Work() override;
	bool TryFindPath();
	bool IsTileValidForWork(const ATile* Tile) const;

	UPROPERTY(VisibleInstanceOnly)
	int32 Size = 0;

public:
	void SetSize(const int32 NewSize);
};
