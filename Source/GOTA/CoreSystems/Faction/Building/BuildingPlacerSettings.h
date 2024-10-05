#pragma once

#include "BuildingPlacerSettings.generated.h"

UCLASS()
class GOTA_API UBuildingPlacerSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* PlacingPossibleMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* PlacingImpossibleMaterial = nullptr;
};
