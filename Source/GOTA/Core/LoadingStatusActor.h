// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingStatusActor.generated.h"

UENUM(BlueprintType)
enum class ELoadingStatus : uint8
{
	NotStarted UMETA(DisplayName = "NotStarted"),
	WaitForAllPlayersReadyToStartLoading UMETA(DisplayName = "WaitForAllPlayersReadyToStartLoading"),
	InitializingGameState UMETA(DisplayName = "InitializingGameState"),
	WaitForAllPlayersReadyForCreation UMETA(DisplayName = "WaitForAllPlayersReadyForCreation"),
	CreateMap UMETA(DisplayName = "CreateMap"),
	CreateFactions UMETA(DisplayName = "CreateFactions"),
	CreateGuardians UMETA(DisplayName = "CreateGuardians"),
	Replicating UMETA(DisplayName = "Replicating"),
	WaitingForAllPlayersReplication UMETA(DisplayName = "WaitingForAllPlayersReplication"),
	InitPlayerControllers UMETA(DisplayName = "InitPlayerControllers"),
	PlayerControllerPossession UMETA(DisplayName = "PlayerControllerPossession"),
	InitializingUI UMETA(DisplayName = "InitializingUI"),
	Finished UMETA(DisplayName = "Finished"),
};


UCLASS()
class GOTA_API ALoadingStatusActor : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedSignature);

	ALoadingStatusActor();
	
public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category="LoadingManager")
	int32 GOTAPlayerID;
	
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	FOnChangedSignature OnChanged;
	
	UPROPERTY(BlueprintReadOnly, Category="LoadingManager")
	int32 ReplicationCount;

	UFUNCTION(BlueprintCallable)
	void IncreaseReplicationCount();
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NetworkedReplicationCount, Category="LoadingManager")
	int32 NetworkedReplicationCount;

	UFUNCTION()
	void OnRep_NetworkedReplicationCount(int32 NewCount);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetNetworkedReplicationCount(const int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentStatus, Category="LoadingManager")
	ELoadingStatus CurrentStatus;

	UFUNCTION()
	void OnRep_CurrentStatus(ELoadingStatus NewStatus);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetCurrentStatus(const ELoadingStatus NewStatus);
};
