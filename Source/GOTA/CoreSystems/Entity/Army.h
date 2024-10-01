// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOTA/CoreSystems/Faction/Building/Population.h"
#include "Army.generated.h"

class AGS_Ingame;
class ATile;
class ASettlement;
class UArmySettings;

UCLASS()
class GOTA_API AArmy : public AActor
{
	GENERATED_BODY()

protected:
	AArmy();

public:
	void Init(ASettlement* InSettlement, ATile* SpawnTile, float InRecruitRate, float InMovementRate, int32 InSize);
	void GOTATick(float DeltaSeconds);

private:
	UPROPERTY(VisibleInstanceOnly)
	EAffiliation Affiliation;

public:
	EAffiliation GetAffiliation() const;

private:
	UPROPERTY(VisibleInstanceOnly)
	ASettlement* Settlement;

	UPROPERTY()
	AGS_Ingame* GameState;

	UPROPERTY()
	UArmySettings* Settings;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleInstanceOnly)
	float Progress; // in percent
	UPROPERTY(VisibleInstanceOnly)
	int32 Size = 0;
	UPROPERTY(VisibleInstanceOnly)
	float RecruitRate; // in percent per second
	UPROPERTY(VisibleInstanceOnly)
	float MovementRate; // in percent per second

	void Move();
	void Recruit();

	// -----------------Status------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	EArmyStatus Status = EArmyStatus::Idle;

	virtual void ValidateStatus();
	bool TryFindPath();
	bool IsTileValidForRecruiting(const ATile* Tile) const;

	EArmyStatus GetStatus() const { return Status; }
	void SetStatus(EArmyStatus NewStatus);

	// -----------------Moving on Path------------------------
	UPROPERTY(VisibleInstanceOnly)
	ATile* CurrentTile;

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;

	// -----------------Combat------------------------
private:
	void CheckForCombat();
	void InitializeCombat(AArmy* Enemy);
	void JoinCombat(AArmy* Enemy);

public:
	void ChallengeToCombat();
};
