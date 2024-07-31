// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GS_Ingame.h"
#include "GameFramework/Actor.h"
#include "LoadingStatusActor.h"
#include "LoadingManager.generated.h"


UCLASS()
class GOTA_API ALoadingManager : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:		
	// index 0 = Server, other indexes the clients with their unique PlayerID
	UPROPERTY(BlueprintReadOnly, Replicated, Category="LoadingManager")
	TArray<ALoadingStatusActor*> LoadingStatuses;

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
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="LoadingManager")
	void Init();

	void IncrementReplicationCount();
};