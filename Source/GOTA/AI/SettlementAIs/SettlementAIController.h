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
	virtual void S_Init(bool RunTraining = false);

	// --------------------------- Possessing ---------------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<ASettlement> PossessedSettlement;

public:
	virtual void Possess(ASettlement* Settlement);
	ASettlement* GetPossessedSettlement() const;
	
	// ----------------------- Reinforcment Learning -----------------------
protected:
	bool bRunTraining = false;
	
	// --------------------------- Log ---------------------------
public:
	virtual void Log();
};
