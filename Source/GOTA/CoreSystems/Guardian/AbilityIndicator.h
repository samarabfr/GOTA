// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityIndicator.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class GOTA_API AAbilityIndicator : public AActor
{
	GENERATED_BODY()
	
	// ---------------------------------------- Lifecycle ----------------------------------------
	
public:
	AAbilityIndicator();
	
	virtual void BeginPlay() override;

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* FX_IndicatorComponent;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem = nullptr;
	
public:
	void Activate();
	void Deactivate();
	
};
