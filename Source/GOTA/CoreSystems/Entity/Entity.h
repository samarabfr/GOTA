// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/GameplayFramework/CombatValues.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Entity.generated.h"

class ATile;

UCLASS()
class GOTA_API AEntity : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatValuesChangedSig, UCombatValues*, NewCombatValues);

	UPROPERTY()
	USplineComponent* Spline;

public:
	AEntity();

	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraPath;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	ATile* CurrentTile;
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	int32 MovementSpeed = 1;

private:
	UPROPERTY(BlueprintGetter=GetPath, Replicated, Category="Entity")
	TArray<ATile*> Path;

	void RefreshSpline();

	// ---------------------------------------------------------
	// Getter & Setter
	
	UFUNCTION(BlueprintGetter)
	TArray<ATile*> GetPath();

	void SetPath(const TArray<ATile*>& NewPath);
};
