// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "Entity.generated.h"

class ATile;

UCLASS()
class GOTA_API AEntity : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AEntity();
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	EAffiliation Affiliation;
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	ATile* Target;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	ATile* CurrentTile;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	TArray<ATile*> Path;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Entity")
	int32 MovementSpeed = 1;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	void Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	virtual void CalculateMovement() PURE_VIRTUAL(ATileEntity::CalculateMovement, );

	UFUNCTION(BlueprintCallable, Category="Entity")
	bool ShouldCombatTrigger() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	void TriggerCombat();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Entity")
	void Kill();
	
	bool IsNextStepBlocked();

	void Step();
};
