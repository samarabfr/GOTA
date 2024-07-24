// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexCoords.h"
#include "GameFramework/Actor.h"
#include "GOTA/EcoSystemDataAsset.h"
#include "GOTA/Faction/GOTAAttributeLimited.h"
#include "Tile.generated.h"

class UBuilding;
class ASettlement;

UCLASS()
class GOTA_API ATile : public AActor
{
	//Unreal Engine Mystery Code
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Constructor
	ATile();

	virtual void BeginPlay() override;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

public:
	UPROPERTY(BlueprintReadOnly)
	FHexCoords HexCoords;

	//====================================================================
	//--------------------Overrideable Functions
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TileMap")
	void Init();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Claim(const ASettlement* PotentialClaimant, bool& Success);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Unclaim();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Build(TSubclassOf<UBuilding> BuildingClass, bool& Success);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, Category="Tile")
	void Unbuild();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintCosmetic, Category="Tile")
	void OnEnteringActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintCosmetic, Category="Tile")
	void OnLeavingActiveRangeOfGuardian();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void CalculateTurn();
	
	void CalculateTreeGrowthChange();

	UFUNCTION()
	void CalculateTreeGrowthChangeWithNeighbors(int32 Change);
	
	void CalculateForageChange();

	UFUNCTION()
	void CalculateForageChangeWithNeighbors(int32 Change);
	
	void CalculateWildlifeGrowthChange();

	UFUNCTION()
	void CalculateWildlifeGrowthChangeWithNeighbors(int32 Change);

	//====================================================================
	//--------------------Bool Flags
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	UPROPERTY(BlueprintReadWrite, Category="Tile")
	bool IsWalkable;

	UPROPERTY(BlueprintReadWrite, Category="Tile")
	bool IsClaimable;

	//====================================================================
	//--------------------Building
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Tile")
	UEcoSystemDataAsset* BalanceData;
	
public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category="Tile")
	UBuilding* Building;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Tile")
	void AddBuildingToReplication();

	//====================================================================
	//--------------------Claimant
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
private:
	UPROPERTY(BlueprintSetter=SetClaimant, BlueprintGetter=GetClaimant, ReplicatedUsing=OnRep_Claimant, Category="Tile")
	ASettlement* Claimant;

	UFUNCTION()
	void OnRep_Claimant(ASettlement* NewClaimant);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Tile")
	void ClaimantChanged();

public:
	UFUNCTION(BlueprintGetter)
	ASettlement* GetClaimant();

	UFUNCTION(BlueprintSetter)
	void SetClaimant(ASettlement* NewClaimant);

	//====================================================================
	//--------------------Attributes
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttributeLimited* Trees;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* TreeGrowth;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* TreeGrowthChange;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttributeLimited* Forage;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* ForageChange;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttributeLimited* Wildlife;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* WildlifeGrowth;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	UGOTAAttribute* WildlifeGrowthChange;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Tile")
	TArray<ATile*> Neighbors; // 0 = North, 1 = NorthEast, 2 = SouthEast, 3 = South, 4 = SouthWest, 5 = NorthWest

	static bool bFreezeGrowthChanges;
};