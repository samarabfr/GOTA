// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GS_Ingame.h"
#include "GameFramework/GameMode.h"
#include "GM_Ingame.generated.h"

UCLASS()
class GOTA_API AGM_Ingame : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(BlueprintReadWrite, Category="GOTA GameMode")
	AGS_Ingame* GOTAGameState;

	UPROPERTY(EditDefaultsOnly, Category="GOTA GameMode")
	TSubclassOf<ATileMap> TileMapClass;

	UPROPERTY(EditDefaultsOnly, Category="GOTA GameMode")
	TSubclassOf<ASettlement> ColonistSettlementClass;

	UPROPERTY(EditDefaultsOnly, Category="GOTA GameMode")
	TSubclassOf<ASettlement> NativeSettlementClass;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void Init();

	virtual void Tick(float DeltaSeconds) override;

	// ---------------------------------------------------------
	// World Setup

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void CreateWorld();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void CreateFactions();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void CreateGuardians();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void InitialPlayerControllerPossession();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GOTA GameMode")
	void StartGame();

	// ---------------------------------------------------------
	// Calculate Turn
private:
	void CalculateTurn();
	FDateTime StartedCalculatingTurn;
};
