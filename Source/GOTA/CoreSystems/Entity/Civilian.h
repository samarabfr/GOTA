#pragma once

#include "GOTA/CoreSystems/Utility/Enums.h"
#include "Civilian.generated.h"

class AGS_Ingame;
class ATile;
class ASettlement;
class UCivilianSettings;
class UBuilding;

UCLASS()
class GOTA_API ACivilian : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	ACivilian();

public:
	void Init(UBuilding* InBuilding, ATile* SpawnTile);
	void GOTATick(float DeltaSeconds);

protected:	
	UPROPERTY(Replicated)
	UBuilding* Building;
	
	UPROPERTY(Replicated)
	UCivilianSettings* Settings;
	
	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;
	
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly)
	float Progress;
	
	// How fast the progress increases when working, in percent per second
	UPROPERTY(VisibleInstanceOnly)
	float WorkRate;

	// How much impact the Work has, for example when producing resources how many resources get produced
	UPROPERTY(VisibleInstanceOnly)
	int32 WorkAmount;

	// How fast the progress increases when moving, in percent per second
	UPROPERTY(VisibleInstanceOnly)
	float MovementRate; 

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
