#pragma once

#include "Civilian.h"
#include "Woodcutter.generated.h"

UCLASS()
class GOTA_API AWoodcutter : public ACivilian
{
	GENERATED_BODY()
	
	virtual void ValidateStatus() override;
	virtual void Work() override;
	bool TryFindPath();
};
