// copyright: none


#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"


UHealthBarComponent::UHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	if (HealthBarWidget) 
	{
		if(HealthBarWidget->HealthBar)
		{
			HealthBarWidget->HealthBar->SetPercent(Percent);
		}
	}
	
	
}

