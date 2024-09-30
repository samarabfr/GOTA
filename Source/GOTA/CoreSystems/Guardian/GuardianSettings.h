#pragma once

#include "Engine/DataAsset.h"
#include "GuardianSettings.generated.h"

class AGuardian;

UCLASS()
class GOTA_API UGuardianSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGuardian> GuardianBlueprint;
	
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;
};
