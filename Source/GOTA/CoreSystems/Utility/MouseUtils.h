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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	APC_Ingame* PlayerController = nullptr;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverTileChangedSig, ATile*, NewTile);

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverActorChangedSig, AActor*, NewActor);
	
	UPROPERTY(BlueprintGetter=GetHoverTile)
	ATile* HoverTile = nullptr;

	UPROPERTY()
	AActor* HoverActor = nullptr;

public:
	UPROPERTY(BlueprintAssignable, Category="MouseUtils")
	FHoverTileChangedSig OnHoverTileChanged;

	FHoverActorChangedSig OnHoverActorChanged;

	UFUNCTION(BlueprintGetter)
	ATile* GetHoverTile() const;

	AActor* GetHoverActor() const { return HoverActor; }

	FVector GetMouseLocation() const;

	FVector GetMouseTileLocation() const;

	void SetPlayerController(APC_Ingame* PC);

	UFUNCTION(BlueprintCallable, Category="MouseUtils")
	void AttachActorToTilePosition(AActor* Actor);

	// ------------------ Mouse Location ------------------

private:
	UPROPERTY(VisibleInstanceOnly)
	USceneComponent* MouseLocation;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_NetMouseLocation)
	FVector NetMouseLocation;

	void SetMouseLocation(const FVector Location);

	UFUNCTION(Server, Unreliable)
	void SRPC_SetNetMouseLocation(const FVector Location);

	UFUNCTION()
	void OnRep_NetMouseLocation();

	// ------------------ Mouse Tile Location ------------------
	
private:
	UPROPERTY(VisibleInstanceOnly)
	USceneComponent* MouseTileLocation;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_NetMouseTileLocation)
	FVector NetMouseTileLocation;

	void SetMouseTileLocation(const FVector Location);

	UFUNCTION(Server, Unreliable)
	void SRPC_SetNetMouseTileLocation(const FVector Location);

	UFUNCTION()
	void OnRep_NetMouseTileLocation();
};
