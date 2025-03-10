// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LoadingManager.generated.h"

enum class ELoadingStatus : uint8;
class AGS_Ingame;
class AGM_Ingame;
class APC_Ingame;
class ALoadingStatusActor;

UCLASS()
class GOTA_API ALoadingManager : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	ALoadingManager();
	
public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="LoadingManager")
	TArray<ALoadingStatusActor*> LoadingStatuses;

	void Delete();
	
private:
	double GracePeriodTime;
	
	UPROPERTY()
	ALoadingStatusActor* LoadingStatus = nullptr;
	
	UPROPERTY()
	APC_Ingame* LocalPlayerController = nullptr;
	
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

	bool IsEveryoneOn(ELoadingStatus Status);
	
public:
	void IncrementReplicationCount();
	
};
