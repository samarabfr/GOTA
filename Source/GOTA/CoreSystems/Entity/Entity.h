// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/GameplayFramework/CombatValues.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
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
	
public:
	AEntity();

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

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	TArray<ATile*> Path;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	int32 MovementSpeed = 1;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	virtual void Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	virtual void CalculateMovement() PURE_VIRTUAL(ATileEntity::CalculateMovement,);

	UFUNCTION(BlueprintCallable, Category="Entity")
	virtual int32 GetAttack() const;

	UFUNCTION(BlueprintCallable, Category="Entity")
	virtual int32 GetDefense() const;

	UFUNCTION(BlueprintCallable, Category="Entity")
	virtual int32 GetHP() const;
	
	UFUNCTION(BlueprintCallable, Category="Entity")
	virtual void DealDamage(int32 Damage);
	
	UFUNCTION(BlueprintCallable, Category="Entity")
	bool ShouldCombatTrigger() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	void Kill();

	bool IsNextStepBlocked();

	void Step();
};
