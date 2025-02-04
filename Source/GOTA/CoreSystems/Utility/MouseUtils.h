#pragma once

#include "GameFramework/Actor.h"
#include "GOTA/CoreSystems/Guardian/AbilityTarget.h"
#include "GOTA/CoreSystems/Tile/Tile.h"
#include "MouseUtils.generated.h"

class APC_Ingame;

UCLASS()
class GOTA_API AMouseUtils : public AActor
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ------------------ LifeCycle ------------------

	AMouseUtils();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	APC_Ingame* PlayerController = nullptr;

public:
	void SetPlayerController(APC_Ingame* PC);
	
	// ---------------------------------------- Utility ----------------------------------------
private:
	UPROPERTY()
	UStaticMeshComponent* TestCube;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* TestCubeMesh;
	
	// ------------------ Mouse Location ------------------

private:
	UPROPERTY(VisibleInstanceOnly)
	USceneComponent* MouseLocation;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_NetMouseLocation)
	FVector NetMouseLocation;

	FVector GetMouseLocation() const { return MouseLocation->GetRelativeLocation(); }

	void SetMouseLocation(const FVector Location);

	UFUNCTION(Server, Unreliable)
	void SRPC_SetNetMouseLocation(const FVector Location);

	UFUNCTION()
	void OnRep_NetMouseLocation();
	
	// ------------------ Hover Tile ------------------
	
private:
	UPROPERTY(VisibleInstanceOnly)
	USceneComponent* MouseTileLocation;
	
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverTileChangedSig, ATile*, NewTile);

	UPROPERTY(ReplicatedUsing=OnRep_HoverTile)
	ATile* HoverTile = nullptr;

	void SetHoverTile(ATile* NewTile);

	UFUNCTION(Server, Reliable)
	void SRPC_SetHoverTile(ATile* NewTile);
	
	UFUNCTION()
	void OnRep_HoverTile();

	void HoverTileChanged();
	
public:
	ATile* GetHoverTile() const { return HoverTile; }
	
	FHoverTileChangedSig OnHoverTileChanged;
	
	void AttachActorToTilePosition(AActor* Actor);

	// ------------------ Hover Actor ------------------
private:
	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverActorChangedSig, AActor*, NewActor);

	UPROPERTY()
	AActor* HoverActor = nullptr;

public:
	FHoverActorChangedSig OnHoverActorChanged;

	AActor* GetHoverActor() const { return HoverActor; }

	FAbilityTarget GetHoverAbilityTarget() const;
};
