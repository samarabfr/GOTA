// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_MainMenu.generated.h"

class ULevelStreamingDynamic;
/**
 * 
 */
UCLASS()
class GOTA_API AGM_MainMenu : public AGameModeBase
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly)
	UWorld* Level = nullptr;
	
public:
	UFUNCTION(BlueprintCallable)
	void StartGame(const bool StartAsListenServer);

	UFUNCTION(BlueprintCallable)
	void JoinGame(FString IP);
};
