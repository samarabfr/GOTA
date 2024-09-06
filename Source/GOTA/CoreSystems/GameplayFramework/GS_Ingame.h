// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem.h"
#include "StartParameter.h"
#include "TotalPopulation.h"
#include "GameFramework/GameState.h"
#include "GOTA/CoreSystems/Entity/Entity.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttribute.h"
#include "GOTA/CoreSystems/Faction/Settlement/SettlementPopulation.h"
#include "GOTA/CoreSystems/Guardian/Guardian.h"
#include "GOTA/CoreSystems/Tile/TileMap.h"
#include "GOTA/CoreSystems/Utility/StaticMeshBatcher.h"
#include "GS_Ingame.generated.h"

class ALoadingManager;

UCLASS()
class GOTA_API AGS_Ingame : public AGameState
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	AGS_Ingame();
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	// ---------------------------------------------------------
	// Stuff in the World

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTAGameState")
	ATileMap* TileMap;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	TArray<ASettlement*> ColonistsSettlements;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	TArray<ASettlement*> NativeSettlements;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	TArray<AGuardian*> Guardians;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	TArray<AEntity*> TileEntities;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	UGOTAAttribute* TotalTrees;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	UGOTAAttribute* TotalForage;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	UGOTAAttribute* TotalWildlife;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	UTotalPopulation* TotalColonialPopulation;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	UTotalPopulation* TotalNativePopulation;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	int32 IslandMaxTrees;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	int32 IslandMaxWildlife;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	int32 IslandMaxForage;

	// ---------------------------------------------------------
	// Turn Stuff
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZeroParamSignature);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTimeChangedSignature, float, ElapsedTime, float, MaxTime);

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FZeroParamSignature OnTurnCounterChanged;

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FTimeChangedSignature OnTurnTimerChanged;

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FZeroParamSignature OnTurnCalculationStart;

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FZeroParamSignature OnTurnCalculationEnd;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	float ElapsedTurnTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category="GOTAGameState")
	float MaxTurnTime;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTAGameState")
	bool IsCalculatingTurn = false;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTAGameState")
	bool ShouldTickTurnTime = false;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TurnCounter, Category="GOTAGameState")
	int32 TurnCounter = 1;

	void IncreaseTurnCounter();

	UFUNCTION()
	void OnRep_TurnCounter();

	UFUNCTION(BlueprintSetter)
	void SetElapsedTurnTime(float NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void SetElapsedTurnTimeMulticast(float NewValue);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void NextTurn();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void TurnCalculationStart();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void TurnCalculationEnd();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void TogglePause();

	// ---------------------------------------------------------
	// Useful Stuff

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	UStartParameter* StartParameter;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	UCombatSystem* CombatSystem;

	UPROPERTY(BlueprintReadWrite, Category="GOTAGameState")
	ALoadingManager* LoadingManager;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="GOTAGameState")
	AStaticMeshBatcher* StaticMeshBatcher;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="GOTAGameState")
	void RegisterTileForTotalsUpdates(ATile* Tile);
	
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameEndingSignature, EGameEnding, Ending, FString, EndMessage);

	UPROPERTY(BlueprintAssignable, Category="GOTAGameState")
	FGameEndingSignature OnGameEnding;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	bool GameEnded = false;

	UFUNCTION(NetMulticast, Reliable)
	void EndGame(EGameEnding Ending, const FString& EndingMessage);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTAGameState")
	void CountIslandMaxEcoValues();

	UPROPERTY()
	EGameStatus GameStatus = EGameStatus::Lobby;
};
