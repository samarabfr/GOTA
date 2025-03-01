// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_MainMenu.generated.h"

UCLASS()
class GOTA_API AGM_MainMenu : public AGameModeBase
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly)
	FString IslandPath = "/Game/CoreSystems/GameplayFramework/Island";
	
	UPROPERTY(EditDefaultsOnly)
	FString SelfPlayLevelPath = "/Game/SelfPlay/SelfPlay_Island";
	
public:
	UFUNCTION(BlueprintCallable)
	void StartGame(const bool StartAsListenServer);

	UFUNCTION(BlueprintCallable)
	void JoinGame(FString IP);
	
	UFUNCTION(BlueprintCallable)
	void StartSelfPlay();
};
