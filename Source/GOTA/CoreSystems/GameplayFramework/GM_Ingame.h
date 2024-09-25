// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameBalance.h"
#include "CoreMinimal.h"
#include "GS_Ingame.h"
#include "GameFramework/GameMode.h"
#include "GM_Ingame.generated.h"

UCLASS()
class GOTA_API AGM_Ingame : public AGameMode
{
	GENERATED_BODY()
	AGM_Ingame();

public:
	UPROPERTY()
	UGameBalanceDataAsset* GameBalance;

	UPROPERTY(BlueprintReadWrite, Category="GOTA GameMode")
	AGS_Ingame* GOTAGameState;

	UPROPERTY(EditDefaultsOnly, Category="GOTA GameMode")
	TSubclassOf<ATileMap> TileMapClass;
	
	UPROPERTY(EditDefaultsOnly, Category="GOTA GameMode")
	TSubclassOf<AMouseUtils> MouseUtilsClass;
	
	// ---------------------------------------------------------
	// Control the Flow of the Game
private:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	                      FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void LoadGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void StartGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void TogglePause();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void PauseGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void UnpauseGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void EndGame(EGameEnding Ending, const FString& EndingMessage);

private:
	void CheckGameEndingConditions();

	// ---------------------------------------------------------
	// World Setup
public:
	void CreateWorld();

	void CreateSettlements();

	void CreateGuardians();

	void CreateMouseUtils();

	void InitialPossession();
};
