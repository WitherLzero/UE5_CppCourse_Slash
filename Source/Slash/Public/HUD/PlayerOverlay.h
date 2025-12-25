// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SLASH_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthPrecent(float Percent);
	void SetStaminaPrecent(float Percent);
	void SetCoinText(int32 Coins);
	void SetSoulText(int32 Souls);
	
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulText;
	
};
