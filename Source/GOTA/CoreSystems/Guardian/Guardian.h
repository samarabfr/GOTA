// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GOTA/CoreSystems/Utility/MouseUtils.h"
#include "Guardian.generated.h"

UCLASS()
class GOTA_API AGuardian : public ACharacter
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMouseUtils> MouseUtilsClass;

	UPROPERTY(BlueprintGetter=GetMouseUtils, Replicated)
	AMouseUtils* MouseUtils;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category="PlayerController")
	void SetupGAM();

	// ---------------------------------------------------------
	// Getter & Setter
	
	UFUNCTION(BlueprintGetter)
	AMouseUtils* GetMouseUtils();
};
