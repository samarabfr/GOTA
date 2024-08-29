#pragma once

#include "CoreMinimal.h"
#include "PlayerSlot.h"
#include "Blueprint/UserWidget.h"
#include "GOTA/CoreSystems/GameplayFramework/GS_Ingame.h"
#include "Lobby.generated.h"

UCLASS(Blueprintable)
class GOTA_API ULobby : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Lobby")
	AGS_Ingame* GameState;

	UPROPERTY(BlueprintReadWrite, Category="Lobby")
	TArray<UPlayerSlot*> PlayerSlots;
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
