// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	// Current Health
	UPROPERTY( EditAnywhere, Category = "Attributes")
	float Health;
	
	UPROPERTY( EditAnywhere, Category = "Attributes")
	float MaxHealth;
public:
	void ReceiveDamage(float Damage);
	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }
	FORCEINLINE bool IsAlive() const { return Health > 0.0f; }	
};
