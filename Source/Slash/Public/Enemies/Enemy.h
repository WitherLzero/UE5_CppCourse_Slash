// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Framework/BaseCharacter.h"
#include "Enemy.generated.h"

class ASoul;
class AAIController;
class UPawnSensingComponent;
class UHealthBarComponent;



UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	/* <AActor> */
	virtual void Tick(float DeltaTime) override;
	/* </AActor> */
	
protected:
	/* <AActor> */
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/* </AActor> */

	/* <ABaseCharacter> */
	// Combat
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void Die() override;
	virtual void HandleDeathEnd(float AnimDuration) override;
	virtual void UpdateHealthUI() const override;
	// Play Montage
	virtual void PlayAttackMontage() override;
	/* </ABaseCharacter> */
	
	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter) override;
	/* </IHitInterface> */
	
	

	
private:
	/* Main Logic */
	UFUNCTION()
	void PawnSeen(APawn* Pawn); // callback
	void HandleCombat();
	void HandlePatrol();

	/* Helpers */
	// Health
	void ShowHealthBar(bool bShow);
	// Behavior
	void Initialize();
	AActor* SelectPatrolTarget();
	void MoveToTarget(AActor* Target) const;
	void StartPatrolling();
	void StartChasing();
	void StartAttacking();
	// Timer relevant
	void DeathEnd(); 
	void PatrolWaitEnd(); 
	void ClearTimer(FTimerHandle& TimerHandle) const;
	// Bool Condition
	bool InTargetRange(AActor* Target, double Radius) const;
	FORCEINLINE bool IsOutsideCombatRadius() const { return !InTargetRange(CombatTarget,CombatRadius); }
	FORCEINLINE bool IsOutsideAttackRadius() const { return !InTargetRange(CombatTarget,AttackRadius); }
	FORCEINLINE bool IsInsideAttackRadius() const { return InTargetRange(CombatTarget,AttackRadius); }
	FORCEINLINE bool IsDead() const { return EnemyState == EEnemyState::EES_Dead;}
	FORCEINLINE bool IsPatrolling() const { return EnemyState == EEnemyState::EES_Patrolling;}
	FORCEINLINE bool IsChasing() const { return EnemyState == EEnemyState::EES_Chasing; }
	FORCEINLINE bool IsAttackWindow() const { return EnemyState == EEnemyState::EES_AttackWindow; }
	FORCEINLINE bool IsAttacking() const { return EnemyState == EEnemyState::EES_Attacking; }

    /* Variables */
	// Property
	UPROPERTY(VisibleInstanceOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	UPROPERTY(EditAnywhere, Category= Properties)
	TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY(EditAnywhere, Category= Properties)
	TSubclassOf<ASoul> SoulClass;
	// Components
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarComponent;
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;
	
	
	// AI Navigation
	UPROPERTY()
	AAIController* EnemyController;
	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	TArray<AActor*> PatrolTargets;
	// Combat
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double CombatRadius = 500.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double AttackRadius = 200.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double PatrollingSpeed = 125.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	double ChasingSpeed = 300.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	float AttackMin = 0.5f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	float AttackMax = 1.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	int32 SoulsReward = 5;
	UPROPERTY(EditAnywhere,Category="Combat")
	float DeathLifeSpan = 3.f;
	
	// Timers
	FTimerHandle DeathTimer;
	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;

public:
};

