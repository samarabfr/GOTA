// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileEntity.generated.h"

class ATile;

UCLASS()
class GOTA_API ATileEntity : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="TileEntity")
	void Move();

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	ATile* Target;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	ATile* CurrentTile;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	TArray<ATile*> Path;
};
