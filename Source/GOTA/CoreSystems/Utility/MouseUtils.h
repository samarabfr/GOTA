#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "MouseUtils.generated.h"

class APC_Ingame;

UCLASS()
class GOTA_API AMouseUtils : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	AMouseUtils();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	TWeakObjectPtr<APC_Ingame> PlayerController = nullptr;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverTileChangedSig, ATile*, NewTile);

	UPROPERTY()
	USceneComponent* MouseLocation;

	UPROPERTY()
	USceneComponent* MouseTileLocation;

	UPROPERTY(BlueprintGetter=GetHoverTile)
	ATile* HoverTile = nullptr;

public:
	UPROPERTY(BlueprintAssignable, Category="MouseUtils")
	FHoverTileChangedSig OnHoverTileChanged;

	UFUNCTION(BlueprintGetter)
	ATile* GetHoverTile() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MouseUtils")
	FVector GetMouseLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MouseUtils")
	FVector GetMouseTileLocation() const;

	void SetPlayerController(APC_Ingame* PC);

	UFUNCTION(BlueprintCallable, Category="MouseUtils")
	void AttachToTilePosition(AActor* Actor);

	// ---------------------------------------------------------
	// Replicated Locations to prevent Lag on MouseUtils
private:
	UPROPERTY(ReplicatedUsing=OnRep_NetLocation)
	FVector NetLocation;

	UFUNCTION()
	void OnRep_NetLocation();

	UFUNCTION(Server, Unreliable)
	void SetNetLocation(FVector Location);

	UPROPERTY(ReplicatedUsing=OnRep_NetTileLocation)
	FVector NetTileLocation;

	UFUNCTION()
	void OnRep_NetTileLocation();

	UFUNCTION(Server, Unreliable)
	void SetNetTileLocation(FVector Location);
};
