// copyright: none


#include "HUD/PlayerOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::SetHealthPrecent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetStaminaPrecent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetCoinText(int32 Coins)
{
	if (CoinText)
	{
		const FString String = FString::Printf(TEXT("%d"),Coins);
		CoinText->SetText(FText::FromString(String));
	}
}

void UPlayerOverlay::SetSoulText(int32 Souls)
{
	if (SoulText)
	{
		const FString String = FString::Printf(TEXT("%d"),Souls);
		SoulText->SetText(FText::FromString(String));
	}
}


