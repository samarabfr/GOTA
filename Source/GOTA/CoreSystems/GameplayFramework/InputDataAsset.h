// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(Blueprintable)
class GOTA_API UInputDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Mapping Context")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, Category="General Actions")
	UInputAction* LeftClick;

	UPROPERTY(EditDefaultsOnly, Category="General Actions")
	UInputAction* Cancel;

	UPROPERTY(EditDefaultsOnly, Category="General Actions")
	UInputAction* Move;
	
	UPROPERTY(EditDefaultsOnly, Category="General Actions")
	UInputAction* Jump;
	
	UPROPERTY(EditDefaultsOnly, Category="General Actions")
	UInputAction* LookAround;

	UPROPERTY(EditDefaultsOnly, Category="General Actions")
	UInputAction* ActivateLooking;

	UPROPERTY(EditDefaultsOnly, Category="General Actions")
	UInputAction* Zoom;

	UPROPERTY(EditDefaultsOnly, Category="Interface Actions")
	UInputAction* Escape;
	
	UPROPERTY(EditDefaultsOnly, Category="Interface Actions")
	UInputAction* BuildMenu;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability1;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability2;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability3;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability4;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability5;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability6;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability7;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* Ability8;

	UPROPERTY(EditDefaultsOnly, Category="Ability Actions")
	UInputAction* DebugMenu;
};
