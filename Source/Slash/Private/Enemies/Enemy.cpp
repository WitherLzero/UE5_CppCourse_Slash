// copyright: none


#include "Enemies/Enemy.h"
#include "Items/Weapons/Weapon.h"

#include "Components/CapsuleComponent.h"
#include "Components/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "AIController.h"
#include "Animation/AnimMontage.h"
#include "Items/Pickups/Soul.h"
#include "Kismet/GameplayStatics.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Initialize();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this,&AEnemy::PawnSeen);
	}

	if (GetWorld() && WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		EquippedWeapon->Equipped(this,FName("RightHandSocket"));
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

void AEnemy::GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactLocation, Hitter);
	if (!IsDead())
	{
		ShowHealthBar(true);
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	UpdateHealthUI();
	
	CombatTarget = EventInstigator->GetPawn();
	ClearTimer(PatrolTimer);
	ClearTimer(AttackTimer);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!IsDead())
	{
		if (IsInsideAttackRadius())
		{
			StartAttacking();
		}else if (IsOutsideAttackRadius())
		{
			StartChasing();
		}
	}
	return DamageAmount;	
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Attacking;
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
	EnemyState = EEnemyState::EES_Dead;
	ClearTimer(AttackTimer);
	ClearTimer(PatrolTimer);
	ShowHealthBar(false);
	
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		const FTransform SpawnTransform = FTransform(GetActorRotation(),GetActorLocation());
		ASoul* SpawnedSoul = World->SpawnActorDeferred<ASoul>(SoulClass,SpawnTransform,this,this,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(this->SoulsReward);
			UGameplayStatics::FinishSpawningActor(SpawnedSoul,SpawnTransform);
		}
	}
}


void AEnemy::HandleDeathEnd(float AnimDuration)
{
	GetWorldTimerManager().SetTimer(DeathTimer,this,&AEnemy::DeathEnd,AnimDuration);
}

void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();
	const FName SectionName = SelectRandomMontageSection(AttackMontageSections);
	PlayMontageSection(AttackMontage,SectionName);
}


void AEnemy::PawnSeen(APawn* Pawn)
{
	if (!IsPatrolling() || IsDead()) return;
	if (Pawn->ActorHasTag(FName("Dead"))) return;
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


void AEnemy::ShowHealthBar(bool bShow)
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(bShow);
	}
}

void AEnemy::UpdateHealthUI() const
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercent(GetHealthPercent());
	}
}

void AEnemy::Initialize()
{
	Tags.Add(FName("Enemy"));
	UpdateHealthUI();
	ShowHealthBar(false);
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
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

void AEnemy::ClearTimer(FTimerHandle& TimerHandle) const
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius) const
{
	if (!Target) return false;
	const double Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
	return Distance <= Radius;
}
