// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SettlementAIController.generated.h"

class ASettlement;

UCLASS()
class GOTA_API ASettlementAIController : public AActor
{
	GENERATED_BODY()

	// --------------------------- LifeCycle ---------------------------
protected:
	ASettlementAIController();

public:
	virtual void Delete();

	// --------------------------- Possessing ---------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	ASettlement* PossessedSettlement = nullptr;

public:
	void Possess(ASettlement* Settlement);
	ASettlement* GetPossessedSettlement() const;
};
