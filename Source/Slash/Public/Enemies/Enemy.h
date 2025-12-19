// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"

#include "Slash/DebugMacro.h"
#include "Enemy.generated.h"

class AAIController;
class UHealthBarComponent;
class UAttributeComponent;
class UAnimMontage;

USTRUCT()
struct FHitReactSections
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,category = Montage)
	FName Front = FName("FromFront");
	UPROPERTY(EditDefaultsOnly,category = Montage)
	FName Back = FName("FromBack");
	UPROPERTY(EditDefaultsOnly,category = Montage)
	FName Left = FName("FromLeft");
	UPROPERTY(EditDefaultsOnly,category = Montage)
	FName Right = FName("FromRight");
};

UCLASS()
class SLASH_API AEnemy : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void GetHit_Implementation(const FVector& ImpactLocation) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Play Montage
	void PlayHitReactMontage(const FName SectionName);
	
	void PlayDeathMontage();

private:
	// Combat
	UPROPERTY(EditAnywhere,Category="Combat")
	float DeathLifeSpan = 3.f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	AActor* CombatTarget;
	
	// Navigation
	UPROPERTY()
	AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	AActor* PatrolTarget;
	
	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	TArray<AActor*> PatrolTargets;
	
	// Timers
	FTimerHandle DeathTimer;
	FTimerHandle PatrolTimer;
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarComponent;
	
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly,category = Montage)
	FHitReactSections HitReactSections;
	
	UPROPERTY(EditAnywhere,category = Sound)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere,category = VisualEffects)
	UParticleSystem* HitParticles;
	

	double CalculateImpactAngle(const FVector& ImpactLocation);
	void DirectionalHitReact(double Theta);
	
	void MoveToTarget(AActor* Target) const;
	AActor* SelectPatrolTarget();
	
	// Timer callbacks
	void DeathEnd();
	void PatrolWaitEnd();

	
	
	
public:
};
