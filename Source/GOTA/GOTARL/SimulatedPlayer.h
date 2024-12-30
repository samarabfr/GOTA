// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimulatedPlayer.generated.h"


class ATile;

UCLASS(Blueprintable)
class GOTA_API ASimulatedPlayer : public AActor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	ASimulatedPlayer();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void S_Init();

	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

private:
	virtual void BeginDestroy() override;


};