// copyright: none


#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	UpdateHealth();
	ShowHealthBar(false);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this,&AEnemy::PawnSeen);
	}

	if (GetWorld() && WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		EquippedWeapon->Equipped(this,FName("RightHandSocket"));
	}
	
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// debug
	DrawDebugSphere(GetWorld(),GetActorLocation(),AttackRadius,12,FColor::Red,false);
	DrawDebugSphere(GetWorld(),GetActorLocation(),CombatRadius,12,FColor::Blue,false);
	
	if (!IsDead())
	{
		if (!IsPatrolling())
		{
			HandleCombat();
		}
		else
		{
			HandlePatrol();
		}
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactLocation)
{
	ShowHealthBar(true);
	if (!IsDead())
	{
		double Theta = CalculateImpactAngle(ImpactLocation);
		DirectionalHitReact(Theta);
	}
	else
	{
		Die();
	}

	PlayHitSound(ImpactLocation);
	SpawnHitParticles(ImpactLocation);
	
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	ClearTimer(PatrolTimer);
	if (!IsDead())
	{
		CombatTarget = EventInstigator->GetPawn();
		StartChasing();
	}
	return DamageAmount;	
}

void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Attacking;
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_Unoccupied;
	Super::AttackEnd();
	HandleCombat();
}

void AEnemy::Die()
{
	Super::Die();
	ClearTimer(AttackTimer);
	ClearTimer(PatrolTimer);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShowHealthBar(false);
	PlayDeathMontage();
}

void AEnemy::HandleDamage(float Damage)
{
	Super::HandleDamage(Damage);
	if (!Attributes->IsAlive()) EnemyState = EEnemyState::EES_Dead;
	UpdateHealth();
}

void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();
	const FName SectionName = SelectRandomMontageSection(AttackMontageSections);
	PlayMontageSection(AttackMontage,SectionName);
}

void AEnemy::PlayDeathMontage()
{
	
	// TO-DO: Fix play death montage twice while getting hit twice
	const FName SectionName = SelectRandomMontageSection(DeathMontageSections);
	const bool bPlayed = PlayMontageSection(DeathMontage,SectionName);
	
	if (bPlayed)
	{
		const float SectionTime = GetMontageSectionDuration(DeathMontage,SectionName);
		GetWorldTimerManager().SetTimer(DeathTimer,this,&AEnemy::DeathEnd,SectionTime);
	}
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	//TO DO: Implement combat logic when notice players
	if (!IsPatrolling() || IsDead()) return;
	if (Pawn->ActorHasTag(FName("Player")))
	{
		CombatTarget = Pawn;
		if (!IsOutsideCombatRadius())
		{
			ClearTimer(PatrolTimer);
			StartChasing();
		}
	}
}


void AEnemy::HandleCombat()
{
	if (IsOutsideCombatRadius())
	{
		ClearTimer(AttackTimer);
		CombatTarget = nullptr;
		if (!IsAttacking())
		StartPatrolling();
	}else if ( IsOutsideAttackRadius() && !IsChasing())
	{
		ClearTimer(AttackTimer);
		if (!IsAttacking())
		StartChasing();
	}else if ( IsInsideAttackRadius() && !IsAttackWindow() &&!IsAttacking())
	{
		StartAttacking();
	}
}

void AEnemy::HandlePatrol()
{
	if (EnemyController->GetMoveStatus() == EPathFollowingStatus::Idle )
	{
		if (!GetWorldTimerManager().IsTimerActive(PatrolTimer))
		{
			GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolWaitEnd,3.f);
		}
	}
}

void AEnemy::ClearTimer(FTimerHandle& TimerHandle) const
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AEnemy::ShowHealthBar(bool bShow)
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(bShow);
	}
}

void AEnemy::UpdateHealth()
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

bool AEnemy::InTargetRange(AActor* Target, double Radius) const
{
	if (!Target) return false;
	const double Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
	return Distance <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target) const
{
	if (!EnemyController || !Target) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::SelectPatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (auto Target: PatrolTargets)
	{
		if (Target!=PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
		
	const int32 Num = ValidTargets.Num();
	if (Num > 0)
	{
		const int32 Index = FMath::RandRange(0,Num - 1);
		return ValidTargets[Index];
	}
	
	return nullptr;
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::StartChasing()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemy::StartAttacking()
{
	EnemyState = EEnemyState::EES_AttackWindow;
	const float AttackTime = FMath::RandRange(AttackMin,AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer,this,&AEnemy::Attack,AttackTime);
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;

	SetLifeSpan(DeathLifeSpan);
}

void AEnemy::PatrolWaitEnd()
{
	PatrolTarget = SelectPatrolTarget(); 
	MoveToTarget(PatrolTarget);
}


