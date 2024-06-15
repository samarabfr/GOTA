// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guardian.h"
#include "GOTA/TileMap/TileMap.h"
#include "GameFramework/GameState.h"
#include "GOTA/Faction/TileEntity.h"
#include "GOTA/Faction/Faction.h"
#include "GOTAGameState.generated.h"

class ALoadingManager;

UCLASS()
class GOTA_API AGOTAGameState : public AGameState
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Constructor
	AGOTAGameState();

	virtual void PostInitializeComponents() override;

	//====================================================================
	//--------------------Simple Variables
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite, Category="GOTAGameState")
	ALoadingManager* LoadingManager;
	
	UPROPERTY(BlueprintReadWrite, Category="GOTAGameState")
	ATileMap* TileMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category="GOTAGameState")
	float MaxTurnTime;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	TArray<AFaction*> Factions;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	TArray<ASettlement*> Settlements;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	TArray<AGuardian*> Guardians;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	TArray<ATileEntity*> TileEntities;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTAGameState")
	bool IsCalculatingTurn;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTAGameState")
	bool ShouldTickTurnTime = false;

	//====================================================================
	//--------------------ElapsedTurnTime
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintGetter=GetElapsedTurnTime, BlueprintSetter=SetElapsedTurnTime,
		Category="GOTAGameState")
	float ElapsedTurnTime;

	UFUNCTION(BlueprintGetter)
	float GetElapsedTurnTime();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MulticastSetElapsedTurnTime(float NewValue);

protected:
	UFUNCTION(BlueprintSetter)
	void SetElapsedTurnTime(float NewValue);

	//====================================================================
	//--------------------Delegates
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZeroParamSignature);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTimeChangedSignature, float, ElapsedTime, float, MaxTime);

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FTimeChangedSignature TurnTimerChanged;

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FZeroParamSignature TurnCalculationStart;

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FZeroParamSignature TurnCalculationEnd;

	//====================================================================
	//-------------------- Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void AddFaction(AFaction* NewFaction);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void AddTileEntity(ATileEntity* NewTileEntity);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void CallCalculationStart();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void CallCalculationEnd();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void Init();
	
	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="GOTAGameState")
	void RegisterTileForTotalsUpdates(ATile* Tile);
	//====================================================================
	//-------------------- Attributes
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileMap")
	UGOTAAttribute* TotalTrees;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileMap")
	UGOTAAttribute* TotalForage;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="TileMap")
	UGOTAAttribute* TotalWildlife;
};