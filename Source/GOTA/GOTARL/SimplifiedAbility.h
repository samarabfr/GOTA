// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimplifiedAbility.generated.h"


class ATile;

UCLASS(Blueprintable)
class GOTA_API ASimplifiedAbility : public AActor
{
	GENERATED_BODY()

	// ----------------------- LifeCycle -----------------------
protected:
	ASimplifiedAbility();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void S_Init();

	void S_Tick(const float DeltaSeconds);
	void C_Tick(const float DeltaSeconds);

private:
	virtual void BeginDestroy() override;

	// ----------------------- Cooldown -----------------------
private:
	UPROPERTY(VisibleInstanceOnly)
	float CooldownLeft = 0.0f;
protected:
	float Cooldown = 60.0f;
	void ActivateCooldown();
	
	// ----------------------- Usage -----------------------
protected:
	int32 TileRange = 4;
	
public:
	bool CanBeUsed(const ATile* Target, const ATile* PlayerPosition) const;
	virtual void Use(ATile* Target, ATile* PlayerPosition);
};
