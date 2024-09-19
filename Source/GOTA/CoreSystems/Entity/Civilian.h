#pragma once

#include "Civilian.generated.h"

class UCivilianDataAsset;

UCLASS()
class GOTA_API ACivilian : public AActor
{
	GENERATED_BODY()
protected:
	ACivilian();

	UPROPERTY()
	UCivilianDataAsset* CivilianDataAsset;
	
	UPROPERTY()
	UStaticMeshComponent* Mesh;
	
	float Progress;
	float Rate;
};
