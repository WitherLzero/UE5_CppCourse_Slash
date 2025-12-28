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
	
	UPROPERTY( EditAnywhere, Category = "Attributes")
	float Stamina;
	
	UPROPERTY( EditAnywhere, Category = "Attributes")
	float MaxStamina;
	
	UPROPERTY( EditAnywhere, Category = "Attributes")
	float StaminaRegenRate;
	
	UPROPERTY( VisibleAnywhere, Category = "Attributes")
	int32 Gold = 0;
	
	UPROPERTY( VisibleAnywhere, Category = "Attributes")
	int32 Souls = 0;
public:
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	void RegenStamina(float DeltaTime);
	void AddGold(int32 Amount);
	void AddSouls(int32 Number);
	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }
	FORCEINLINE float GetStaminaPercent() const { return Stamina / MaxStamina; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE bool IsAlive() const { return Health > 0.0f; }	
};
