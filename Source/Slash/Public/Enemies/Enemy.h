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

protected:
	virtual void BeginPlay() override;


	// Combat
	virtual void Attack() override;
	virtual void GetHit_Implementation(const FVector& ImpactLocation) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void HandleDamage(float Damage) override;

	// Play Montage
	virtual void PlayAttackMontage() override;
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;
	
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	// Combat
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	AActor* CombatTarget;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double CombatRadius = 500.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double AttackRadius = 200.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double PatrollingSpeed = 125.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double ChasingSpeed = 300.f;
	UPROPERTY(EditAnywhere,Category="Combat")
	float DeathLifeSpan = 3.f;
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
	// Timer callbacks
	void DeathEnd();
	void PatrolWaitEnd();


	void HandleCombat();
	void HandlePatrol();
	

	// Inner Helpers
	void ShowHealthBar(bool bShow);
	void UpdateHealth();
	
	bool InTargetRange(AActor* Target, double Radius) const;
	void MoveToTarget(AActor* Target) const;
	AActor* SelectPatrolTarget();
	void StartPatrolling();
	void StartChasing();

	FORCEINLINE bool IsOutsideCombatRadius() const { return !InTargetRange(CombatTarget,CombatRadius); }
	FORCEINLINE bool IsOutsideAttackRadius() const { return !InTargetRange(CombatTarget,AttackRadius); }
	FORCEINLINE bool IsInsideAttackRadius() const { return InTargetRange(CombatTarget,AttackRadius); }
	FORCEINLINE bool IsPatrolling() const { return EnemyState == EEnemyState::EES_Patrolling;}
	FORCEINLINE bool IsChasing() const { return EnemyState == EEnemyState::EES_Chasing; }
	FORCEINLINE bool IsAttacking() const { return EnemyState == EEnemyState::EES_Attacking; }
	
	
	
	

public:
};
