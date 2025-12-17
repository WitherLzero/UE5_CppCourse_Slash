// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHealthBarComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
