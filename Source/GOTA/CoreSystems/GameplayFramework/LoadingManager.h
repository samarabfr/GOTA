// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GM_Ingame.h"
#include "GS_Ingame.h"
#include "GameFramework/Actor.h"
#include "LoadingStatusActor.h"
#include "PC_Ingame.h"
#include "LoadingManager.generated.h"

UCLASS()
class GOTA_API ALoadingManager : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	ALoadingManager();
	
public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="LoadingManager")
	TArray<ALoadingStatusActor*> LoadingStatuses;
	
private:
	UPROPERTY()
	ALoadingStatusActor* LoadingStatus = nullptr;
	
	UPROPERTY()
	TWeakObjectPtr<APC_Ingame> LocalPlayerController = nullptr;
	
	UPROPERTY()
	AGM_Ingame* GameMode = nullptr;

	UPROPERTY()
	AGS_Ingame* GameState = nullptr;

	int32 GOTAPlayerID = -1;

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void ServerTick();

	void ClientTick();

	void SpawnLoadingStatuses();
	
public:
	void IncrementReplicationCount();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="LoadingManager")
	void InitLoadingScreen();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="LoadingManager")
	void RemoveLoadingScreen();
};
