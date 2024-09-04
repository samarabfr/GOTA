#pragma once

#include "CoreMinimal.h"
#include "EcoSystemDataAsset.h"
#include "GOTA/CoreSystems/Utility/Enums.h"
#include "EcoValues.generated.h"

UCLASS(Blueprintable)
class GOTA_API UEcoValues : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UEcoValues();

public:
	UPROPERTY(BlueprintReadOnly, Category="Tile")
	UEcoSystemDataAsset* DA_EcoSystem;

public:
	void Init(EBiome Biome);

	void ServerTick(double DeltaSeconds);
	void ClientTick(double DeltaSeconds);
	
	void SetMaxValues(int32 NewMaxTrees, EBiome Biome);
	void MaxALlValues();
	
	void AddNeighborTrees(int32 Amount);
	void AddNeighborWildlife(int32 Amount);
	void AddNeighborForage(int32 Amount);

	void SubtractNeighborTrees(int32 Amount);
	void SubtractNeighborWildlife(int32 Amount);
	void SubtractNeighborForage(int32 Amount);

	// -------------------OnChange-------------------------
private:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcoValueChangedSig, int32, Change);

public:
	UPROPERTY(BlueprintAssignable)
	FOnEcoValueChangedSig OnTreesChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEcoValueChangedSig OnWildlifeChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnEcoValueChangedSig OnForageChanged;

private:
	void OnTreeChanges(int32 Change);
	void OnWildlifeChanges(int32 Change);
	void OnForageChanges(int32 Change);

	// -------------------Definition-------------------------
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTrees, ReplicatedUsing=OnRep_Trees, Category="Tile")
	int32 Trees = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetWildlife, ReplicatedUsing=OnRep_Wildlife, Category="Tile")
	int32 Wildlife = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetForage, ReplicatedUsing=OnRep_Forage, Category="Tile")
	int32 Forage = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetMaxTrees, Replicated, Category="Tile")
	int32 MaxTrees = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetMaxWildlife, Replicated, Category="Tile")
	int32 MaxWildlife = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetMaxForage, Replicated, Category="Tile")
	int32 MaxForage = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTreeGrowthProgress, Replicated, Category="Tile")
	float TreeGrowthProgress = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetWildlifeGrowthProgress, Replicated, Category="Tile")
	float WildlifeGrowthProgress = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetForageGrowthProgress, Replicated, Category="Tile")
	float ForageGrowthProgress = 0.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTreeGrowth, Category="Tile")
	float TreeGrowth = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetWildlifeGrowth, Category="Tile")
	float WildlifeGrowth = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetForageGrowth, Category="Tile")
	float ForageGrowth = 0.0f;

	// -------------------Getter-------------------------
public:
	UFUNCTION(BlueprintCallable, Category="Tile")
	int32 GetTrees() const { return Trees; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	int32 GetWildlife() const { return Wildlife; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	int32 GetForage() const { return Forage; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	int32 GetMaxTrees() const { return MaxTrees; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	int32 GetMaxWildlife() const { return MaxWildlife; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	int32 GetMaxForage() const { return MaxForage; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	float GetTreeGrowthProgress() const { return TreeGrowthProgress; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	float GetWildlifeGrowthProgress() const { return WildlifeGrowthProgress; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	float GetForageGrowthProgress() const { return ForageGrowthProgress; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	float GetTreeGrowth() const { return TreeGrowth; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	float GetWildlifeGrowth() const { return WildlifeGrowth; }

	UFUNCTION(BlueprintCallable, Category="Tile")
	float GetForageGrowth() const { return ForageGrowth; }

	// -------------------Setter-------------------------

	void SetTrees(int32 NewTrees);

	void SetWildlife(int32 NewWildlife);

	void SetForage(int32 NewForage);

	// -------------------OnReps-------------------------
private:
	UFUNCTION()
	void OnRep_Trees(int32 OldValue);

	UFUNCTION()
	void OnRep_Wildlife(int32 OldValue);

	UFUNCTION()
	void OnRep_Forage(int32 OldValue);

	// -------------------Adder-------------------------
public:
	UFUNCTION(BlueprintCallable)
	void AddTrees(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void AddWildlife(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void AddForage(int32 Amount);

	// -------------------Subtract-er-------------------------
	
	UFUNCTION(BlueprintCallable)
	void SubtractTrees(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void SubtractWildlife(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void SubtractForage(int32 Amount);
};
