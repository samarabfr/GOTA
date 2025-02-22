// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "GS_Ingame.generated.h"

class AAIController;
class ULearningAgentsManager;
class ULearningAgentsNeuralNetwork;
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


	virtual void BeginPlay() override;

	void S_Init();

	void C_Init();

protected:
	AGS_Ingame();

	// ------------------- Utility -------------------
public:
	UPROPERTY()
	ADaytimeManager* DaytimeManager;

	UPROPERTY(Replicated)
	UStartParameter* StartParameter;

	void DeleteEverything();

	// ------------------- TileMap -------------------
private:
	UPROPERTY(Replicated)
	ATileMap* TileMap;

public:
	ATileMap* GetTileMap() const { return TileMap; }

	void SetTileMap(ATileMap* NewTileMap);

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

	// TODO: kann man mit Affiliation ersetzen: "WinningTeam"
	EGameEnding GameEnding = EGameEnding::ColonistsWon;

public:
	UPROPERTY()
	EGameStatus GameStatus = EGameStatus::Lobby;

	FGameEndingSignature OnGameEnding;

	bool GameEnded = false;

	UFUNCTION(NetMulticast, Reliable)
	void S_EndGame(EGameEnding Ending, const FString& EndingMessage);

	EGameEnding GetGameEnding() const { return GameEnding; }


	// ------------------- Reinforcement Learning Manager -------------------

private:
	TArray<AAIController*> GuardianAIControllers;

public:
	TArray<AAIController*> GetGuardianAIControllers() const;
	AAIController* GetGuardianAIController(int32 GOTAPlayerID) const;
	void SetGuardianAIController(int32 GOTAPlayerID, AAIController* GuardianAIController);


	// ------------------- Reinforcement Learning Manager -------------------
private:
	TMap<TSubclassOf<AActor>, TWeakObjectPtr<AActor>> RL_Managers;

public:
	template <typename T = ULearningAgentsManager>
	T* S_GetRLManager(TSubclassOf<AActor> ManagerClass)
	{
		if (!RL_Managers.Contains(ManagerClass)) return nullptr;
		return Cast<T>(RL_Managers[ManagerClass].Get());
	}

	void S_AddManager(TSubclassOf<AActor> ManagerClass, AActor* Manager);
};
