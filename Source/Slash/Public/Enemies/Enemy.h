// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Framework/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"

#include "Slash/DebugMacro.h"
#include "_Enums/CharacterTypes.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;
class UAttributeComponent;
class UAnimMontage;



UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
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
	virtual void PlayDeathMontage() override;

	// Callbacks
	UFUNCTION()
	void PawnSeen(APawn* Pawn);
	
private:
	// Components
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarComponent;
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;
	
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	// Combat
	UPROPERTY(EditAnywhere,Category="Combat")
	float DeathLifeSpan = 3.f;
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	AActor* CombatTarget;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double CombatRadius = 500.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double AttackRadius = 200.f;
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
	
	
	void CheckCombatTarget();
	void HandlePatrol();
	
	bool InTargetRange(AActor* Target, double Radius) const;
	void MoveToTarget(AActor* Target) const;
	AActor* SelectPatrolTarget();
	
	// Timer callbacks
	void DeathEnd();
	void PatrolWaitEnd();


public:
};
