// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOTAGameState.h"
#include "GameFramework/Actor.h"
#include "LoadingManager.generated.h"

UENUM(BlueprintType)
enum class ELoadingStatus : uint8
{
	InitializingGameState UMETA(DisplayName = "InitializingGameState"),
	WaitForAllPlayersReadyForCreation UMETA(DisplayName = "WaitForAllPlayersReadyForCreation"),
	CreateMap UMETA(DisplayName = "CreateMap"),
	CreateFactions UMETA(DisplayName = "CreateFactions"),
	CreateGuardians UMETA(DisplayName = "CreateGuardians"),
	WaitingForAllPlayersReplication UMETA(DisplayName = "WaitingForAllPlayersReplication"),
	InitPlayerControllers UMETA(DisplayName = "InitPlayerControllers"),
	PlayerControllerPossession UMETA(DisplayName = "PlayerControllerPossession"),
	InitializingUI UMETA(DisplayName = "InitializingUI"),
	Finished UMETA(DisplayName = "Finished"),
};

UCLASS()
class GOTA_API ALoadingManager : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ALoadingManager();
		
	// index 0 = Server, other indexes the clients with their unique PlayerID
	UPROPERTY(BlueprintReadOnly, Replicated, Category="LoadingManager")
	TArray<int32> ReplicationCounts;

	UPROPERTY(BlueprintReadOnly, Category="LoadingManager")
	int32 LocalReplicationCount;
	
	// index 0 = Server, other indexes the clients with their unique PlayerID
	UPROPERTY(BlueprintReadWrite, Replicated, Category="LoadingManager")
	TArray<ELoadingStatus> CurrentStatus;

	UPROPERTY(BlueprintReadWrite, Category="LoadingManager")
	ELoadingStatus LocalStatus;
	
	UPROPERTY(BlueprintReadWrite, Category="LoadingManager")
	AGOTAGameState* GameState;

	UPROPERTY(BlueprintReadWrite, Category="LoadingManager")
	int32 GOTAPlayerID;


	//====================================================================
	//                     Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="LoadingManager")
	void InitLoadingScreen();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="LoadingManager")
	void RemoveLoadingScreen();

	//====================================================================
	//                            Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	UFUNCTION(BlueprintCallable, Category="LoadingManager")
	void IncreaseReplicationCount();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="LoadingManager")
	void SetReplicationCountRPC(int32 Index, int32 Count);

	UFUNCTION(BlueprintCallable, Category="LoadingManager")
	void Init();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="LoadingManager")
	void SetLoadingStatus(int32 Index, ELoadingStatus Status);
};