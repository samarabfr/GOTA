// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/Utility/Enums.h"
#include "Entity.generated.h"

class UNiagaraComponent;
class USplineComponent;
class UStateTreeComponent;
class AGS_Ingame;
class UBuilding;
class ATile;

UCLASS()
class GOTA_API AEntity : public AActor
{
	GENERATED_BODY()

	// ----------------------- Replication Setup -----------------------
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ----------------------- LifeCycle -----------------------	
protected:
	AEntity();

public:
	UFUNCTION()
	virtual void Delete();

	virtual void S_Init(UBuilding* InBuilding, ATile* SpawnTile);
	virtual void S_Tick(const float DeltaSeconds);
	virtual void C_Tick(const float DeltaSeconds);
	// ----------------------- Utility -----------------------

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	TWeakObjectPtr<UBuilding> OriginBuilding;

	UPROPERTY()
	TWeakObjectPtr<AGS_Ingame> GameState;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Affiliation)
	EAffiliation Affiliation;

	UPROPERTY(EditDefaultsOnly)
	UStateTreeComponent* StateTree;


protected:
	UBuilding* GetOriginBuilding() const;
	AGS_Ingame* S_GetGameState() const;

	UFUNCTION()
	virtual void S_HandleEfficiencyChange(float Change);
	
	UFUNCTION()
	virtual void OnRep_Affiliation();

public:
	virtual EEntityType GetEntityType() const;
	EAffiliation GetAffiliation() const;

	// ----------------- Progresser ------------------------
protected:
	// Progress of current Action in percent
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Progress;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool bProgresserActive = false;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float ProgressRate = 0.f;

	std::function<float()> CalculateProgressRate;
	std::function<void()> FinishProgress;

public:
	void S_StartProgresser(const std::function<float()>& ProgressRateCalculator,
	                       const std::function<void()>& Finisher);
	void S_StopProgresser();
	void ProgressTick(float DeltaSeconds);

	// ----------------- Movement ------------------------
private:
	UPROPERTY(ReplicatedUsing=OnRep_NetLocation)
	FVector NetLocation;

	// How fast the progress increases when moving, in percent per second
	UPROPERTY(VisibleInstanceOnly, Replicated)
	float MovementRate;

	UPROPERTY(VisibleInstanceOnly)
	TArray<ATile*> Path;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	TWeakObjectPtr<ATile> CurrentTile;

protected:
	UFUNCTION()
	void OnRep_NetLocation();

	void SetNetLocation(const FVector& NewNetLocation);

	ATile* GetCurrentTile() const;
	void S_SetMovementRate(float NewMovementRate);

public:
	void S_MoveToNextTileOnPath();
	float GetMovementRate() const;
	void S_SetPath(const TArray<ATile*>& NewPath);
	bool IsPathValid();
	bool IsPathEmpty() const;

	// ----------------- Path Graphics ------------------------
private:
	UPROPERTY()
	USplineComponent* Spline;

	void RefreshSpline();

public:
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraPath;
};
