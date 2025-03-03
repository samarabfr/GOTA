// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingStatusActor.generated.h"

UENUM(BlueprintType)
enum class ELoadingStatus : uint8
{
	NotStarted UMETA(DisplayName = "NotStarted"),
	WaitForReadyForCreation UMETA(DisplayName = "WaitForReadyForCreation"),
	Replicating UMETA(DisplayName = "Replicating"),
	WaitForReplication UMETA(DisplayName = "WaitForReplication"),
	WaitForFinished UMETA(DisplayName = "WaitForFinished"),
	Finished UMETA(DisplayName = "Finished")
};


UCLASS()
class GOTA_API ALoadingStatusActor : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedSignature);

	ALoadingStatusActor();
	
public:
	void Delete();
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Loading")
	int32 GOTAPlayerID;

	UPROPERTY(BlueprintAssignable, Category="Loading")
	FOnChangedSignature OnChanged;

	UPROPERTY(BlueprintReadOnly, Category="Loading")
	int32 RepCount;

	void IncreaseReplicationCount();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NetRepCount, Category="Loading")
	int32 NetRepCount;
private:
	UFUNCTION()
	void OnRep_NetRepCount(int32 NewCount);
public:
	UFUNCTION(Server, Reliable)
	void SetNetRepCount(const int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentStatus, Category="Loading")
	ELoadingStatus CurrentStatus;
private:
	UFUNCTION()
	void OnRep_CurrentStatus(ELoadingStatus NewStatus);
	
public:
	void SetCurrentStatus(const ELoadingStatus NewStatus);
	
private:
	UFUNCTION(Server, Reliable)
	void SetCurrentStatusServer(const ELoadingStatus NewStatus);
};
