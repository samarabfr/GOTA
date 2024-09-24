#pragma once
#include "GOTA/CoreSystems/Utility/Enums.h"

#include "Civilian.generated.h"

class AGS_Ingame;
class ATile;
class ASettlement;
class UCivilianDataAsset;

UCLASS()
class GOTA_API ACivilian : public AActor
{
	GENERATED_BODY()

protected:
	ACivilian();

public:
	void Init(ASettlement* Settlement_, ATile* SpawnTile, float WorkRate_, int32 WorkAmount_, float MovementRate_);
	void GOTATick(float DeltaSeconds);

protected:	
	UPROPERTY(VisibleInstanceOnly)
	ASettlement* Settlement;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UCivilianDataAsset* CivilianDataAsset;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleInstanceOnly)
	float Progress; // in percent
	UPROPERTY(VisibleInstanceOnly)
	float WorkRate; // in percent per second
	UPROPERTY(VisibleInstanceOnly)
	int32 WorkAmount;
	UPROPERTY(VisibleInstanceOnly)
	float MovementRate; // in percent per second

	void Move();
	virtual void Work();

	// -----------------Status------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	ECivilianStatus Status;

protected:
	virtual void ValidateStatus();

	ECivilianStatus GetStatus() const { return Status; }
	void SetStatus(ECivilianStatus NewStatus);

	// -----------------Moving on Path------------------------
	UPROPERTY(VisibleInstanceOnly)
	ATile* CurrentTile;

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;
};
