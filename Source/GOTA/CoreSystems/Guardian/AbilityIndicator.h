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

	virtual void Tick(float DeltaSeconds) override;

	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY(EditDefaultsOnly)
	UMaterial* IconMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* IconMaterialInstance;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Rotator;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StonePlateMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FrontIcon;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackIcon;


	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* FX_IndicatorComponent;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem = nullptr;

public:
	void Activate();
	void Deactivate();
};
