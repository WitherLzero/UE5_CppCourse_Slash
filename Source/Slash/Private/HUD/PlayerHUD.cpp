// copyright: none


#include "HUD/PlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/PlayerOverlay.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && OverlayClass)
		{
			PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller,OverlayClass);
			PlayerOverlay->AddToViewport();
		}
	}
	
}
