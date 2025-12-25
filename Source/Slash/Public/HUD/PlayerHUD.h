// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;
/**
 * 
 */
UCLASS()
class SLASH_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly,Category = HUD)
	TSubclassOf<UPlayerOverlay> OverlayClass;
	
	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};
