// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "GS_Ingame.generated.h"

class ATile;
class UGOTAAttribute;
class AEntity;
class AGuardian;
class ATribe;
class AColony;
class UStartParameter;
class AStaticMeshBatcher;
class ATileMap;
class AGameSettings;
class ADaytimeManager;
class ALoadingManager;

UCLASS()
class GOTA_API AGS_Ingame : public AGameState
{
	GENERATED_BODY()

	// ------------------- Replication Setup -------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddReplicatedSubobjects();

	// ------------------- LifeCycle -------------------

	AGS_Ingame();

	virtual void BeginPlay() override;

	void S_Init();

	void C_Init();

	// ------------------- Utility -------------------
public:
	UPROPERTY()
	ADaytimeManager* DaytimeManager;

	UPROPERTY(Replicated)
	UStartParameter* StartParameter;

	// ------------------- TileMap -------------------
private:
	UPROPERTY(Replicated)
	ATileMap* TileMap;

public:
	ATileMap* GetTileMap() const { return TileMap; }

	void SetTileMap(ATileMap* NewTileMap);

	// ------------------- GameSettings -------------------
private:
	UPROPERTY(Replicated)
	AGameSettings* GameSettings;

	void SpawnGameSettingsActor();

public:
	AGameSettings* GetGameSettings() { return GameSettings; }

	// ------------------- LoadingManager -------------------
private:
	UPROPERTY()
	ALoadingManager* LoadingManager;

public:
	ALoadingManager* GetLoadingManager() const { return LoadingManager; }
	void SetLoadingManager(ALoadingManager* NewLoadingManager);
	void IncrementReplicationCount();

	// ------------------- StaticMeshBatcher -------------------
private:
	UPROPERTY()
	AStaticMeshBatcher* StaticMeshBatcher;

	void SpawnStaticMeshBatcher();

public:
	AStaticMeshBatcher* GetStaticMeshBatcher() { return StaticMeshBatcher; }

	// ------------------- Settlements -------------------
private:
	UPROPERTY(Replicated)
	AColony* Colony;

	UPROPERTY(Replicated)
	ATribe* Tribe;

public:
	AColony* GetColony() const { return Colony; }

	void SetColony(AColony* NewColony);

	ATribe* GetTribe() const { return Tribe; }

	void SetTribe(ATribe* NewTribe);

	// ------------------- Guardians -------------------
private:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuardiansChangedSig, AGS_Ingame*, GameState);

	UPROPERTY(ReplicatedUsing=GuardiansChanged)
	TArray<AGuardian*> Guardians;

	UFUNCTION()
	void GuardiansChanged();

public:
	FOnGuardiansChangedSig OnGuardiansChanged;

	TArray<AGuardian*> GetGuardians() const { return Guardians; }

	AGuardian* GetGuardian(int32 GOTAPlayerID) const;

	void SetGuardian(int32 GOTAPlayerID, AGuardian* Guardian);

	// ------------------- Entities -------------------

	UPROPERTY()
	TArray<AEntity*> TileEntities;

	// ------------------- Island Health -------------------

	void RegisterTileForTotalsUpdates(ATile* Tile);

	void CountIslandMaxEcoValues();

	UPROPERTY(Replicated)
	UGOTAAttribute* TotalTrees;

	UPROPERTY(Replicated)
	UGOTAAttribute* TotalForage;

	UPROPERTY(Replicated)
	int32 IslandMaxTrees;

	UPROPERTY(Replicated)
	int32 IslandMaxForage;

	// ------------------- Game Ending -------------------
private:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
		FGameEndingSignature, const EGameEnding, Ending, const FString&, EndMessage);

public:
	UPROPERTY()
	EGameStatus GameStatus = EGameStatus::Lobby;

	FGameEndingSignature OnGameEnding;

	bool GameEnded = false;

	UFUNCTION(NetMulticast, Reliable)
	void S_EndGame(EGameEnding Ending, const FString& EndingMessage);
};
