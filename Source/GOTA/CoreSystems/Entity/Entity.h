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

	UPROPERTY(BlueprintGetter=GetAffiliation, Replicated, Category="Entity")
	EAffiliation Affiliation;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatValuesChangedSig, UCombatValues*, NewCombatValues);

	UPROPERTY()
	USplineComponent* Spline;

public:
	AEntity();

	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraPath;
	
	UPROPERTY(BlueprintAssignable)
	FOnCombatValuesChangedSig OnCombatValuesChanged;

	UFUNCTION()
	void CombatValuesChanged(UCombatValues* CombatValues);

	UFUNCTION(BlueprintGetter)
	EAffiliation GetAffiliation();

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	ATile* Target;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	ATile* CurrentTile;

private:
	UPROPERTY(BlueprintGetter=GetPath, Replicated, Category="Entity")
	TArray<ATile*> Path;
public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	int32 MovementSpeed = 1;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	virtual void Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	virtual void CalculateMovement() PURE_VIRTUAL(ATileEntity::CalculateMovement,);
	
	UFUNCTION(BlueprintCallable, Category="Entity")
	virtual void KillIndividuals(int32 Kills);

	UFUNCTION(BlueprintCallable, Category="Entity")
	bool ShouldCombatTrigger() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	void Kill();

	bool IsNextStepBlocked();

	void Step();

	void RefreshSpline();

	// ---------------------------------------------------------
	// Getter & Setter
	UFUNCTION(BlueprintCallable, Category="Entity")
	virtual UCombatValues* GetCombatValues() const;
	
	UFUNCTION(BlueprintGetter)
	TArray<ATile*> GetPath();

	void SetPath(const TArray<ATile*>& NewPath);
};
