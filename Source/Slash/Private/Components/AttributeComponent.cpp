// copyright: none


#include "Components/AttributeComponent.h"


UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.0f, MaxStamina);
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime,0.f, MaxStamina);
}

void UAttributeComponent::AddGold(int32 Amount)
{
	Gold = FMath::Max(Gold+Amount,0);
}

void UAttributeComponent::AddSouls(int32 Number)
{
	Souls = FMath::Max(Souls+Number,0);
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

