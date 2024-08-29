#pragma once

#include "StartParameter.generated.h"

UCLASS(Blueprintable)
class UStartParameter : public UObject
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartParameterChangedSig, UStartParameter*, NewParameter);

public:
	UPROPERTY(BlueprintAssignable, Category="StartParameter")
	FStartParameterChangedSig OnChanged;
private:
	UPROPERTY(BlueprintGetter=GetIslandSize,
		BlueprintSetter=SetIslandSize,
		ReplicatedUsing=OnRep_IslandSize,
		Category="StartParameter")
	int32 IslandSize = 800;

	UPROPERTY(BlueprintGetter=GetStartingColonialSettlements,
		BlueprintSetter=SetStartingColonialSettlements,
		ReplicatedUsing=OnRep_StartingColonialSettlements,
		Category = "StartParameter")
	int32 StartingColonialSettlements = 4;
	
public:
	UFUNCTION(BlueprintGetter)
	int32 GetIslandSize();

	UFUNCTION(BlueprintSetter, BlueprintAuthorityOnly)
	void SetIslandSize(int32 NewValue);

	UFUNCTION(BlueprintGetter)
	int32 GetStartingColonialSettlements();

	UFUNCTION(BlueprintSetter, BlueprintAuthorityOnly)
	void SetStartingColonialSettlements(int32 NewValue);
	
	UFUNCTION()
	void OnRep_IslandSize();
	
	UFUNCTION()
	void OnRep_StartingColonialSettlements();
};
