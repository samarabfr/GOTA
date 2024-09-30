// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem.h"
#include "StartParameter.h"
#include "GameFramework/GameState.h"
#include "GOTA/CoreSystems/Entity/Entity.h"
#include "GOTA/CoreSystems/Faction/Attribute/GOTAAttribute.h"
#include "GOTA/CoreSystems/Faction/Settlement/Colony.h"
#include "GOTA/CoreSystems/Faction/Settlement/Tribe.h"
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
	// ---------------------------------------------------------
	// Stuff in the World

	UPROPERTY(BlueprintReadWrite, Replicated, Category="GOTAGameState")
	ATileMap* TileMap;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	AColony* Colony;

	UPROPERTY(BlueprintReadOnly, Category="GOTAGameState")
	ATribe* Tribe;

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

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	int32 IslandMaxTrees;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	int32 IslandMaxWildlife;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="GOTAGameState")
	int32 IslandMaxForage;

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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameEndingSignature,
	                                             const EGameEnding, Ending,
	                                             const FString&, EndMessage);

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
