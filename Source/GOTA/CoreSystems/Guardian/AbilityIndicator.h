// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTarget.h"
#include "GameFramework/Actor.h"
#include "AbilityIndicator.generated.h"

class UGotaImage;
class UWidgetComponent;
class AAbility;
class UGuardianSettings;
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
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Rotator;

	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StonePlateMesh;

	
	UPROPERTY(EditAnywhere)
	UWidgetComponent* FrontIcon;
	
	UPROPERTY(EditAnywhere)
	UWidgetComponent* BackIcon;

	
	UPROPERTY()
	UGotaImage* IconWidget;
	
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* GuardianNiagaraEffect;

public:
	/**
	 * Sets a new Guardian that is using this indicator.
	 */
	void SetGuardian(AGuardian* Guardian);
	
	/**
	 * Sets a new Ability to target. Currently only sets the Icon on the stone plate to represent the Ability
	 */
	void SetAbility(AAbility* Ability);
	
	/**
	 *	Sets a new Target for this indicator. Moves the indicator to the target, scales the graphics to fit
	 *	the target type. In case of a Guardian it will disable the graphics in world and tell the UI to
	 *	indicate which guardian is getting targeted
	 */
	void SetTarget(const FAbilityTarget& NewAbilityTarget);

	/**
	 *	Activates the graphics that represent the targeting of an Ability.
	 */
	void Activate();
	
	/**
	 * Deactivates all graphics of this Indicator
	 */
	void Deactivate();
};
