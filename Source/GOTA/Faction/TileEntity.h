// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "TileEntity.generated.h"

class ATile;

UCLASS()
class GOTA_API ATileEntity : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ATileEntity();
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileEntity")
	EAffiliation Affiliation;
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileEntity")
	ATile* Target;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileEntity")
	ATile* CurrentTile;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileEntity")
	TArray<ATile*> Path;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileEntity")
	int32 MovementSpeed = 1;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileEntity")
	void Init(EAffiliation Affiliation_, ATile* CurrentTile_, int32 MovementSpeed_);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileEntity")
	virtual void CalculateMovement() PURE_VIRTUAL(ATileEntity::CalculateMovement, );

	UFUNCTION(BlueprintCallable, Category="TileEntity")
	bool ShouldCombatTrigger() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileEntity")
	void TriggerCombat();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileEntity")
	void Kill();
	
	bool IsNextStepBlocked();

	void Step();
};
