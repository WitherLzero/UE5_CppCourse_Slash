// copyright: none


#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"


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
	if (HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
	}

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this,&AEnemy::PawnSeen);
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

	if (EnemyState != EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		HandlePatrol();
	}
	
	
}

void AEnemy::GetHit_Implementation(const FVector& ImpactLocation)
{
	if (Attributes->IsAlive())
	{
		double Theta = CalculateImpactAngle(ImpactLocation);
		DirectionalHitReact(Theta);
	}
	else
	{
		PlayDeathMontage();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,HitSound,ImpactLocation);
	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,HitParticles,ImpactLocation);
	}
	
	
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
		if (HealthBarComponent)
		{
			HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
		}
	}
	return DamageAmount;	
}

void AEnemy::PlayHitReactMontage(const FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName,HitReactMontage);
	}
}

void AEnemy::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		const int NumSections = DeathMontage->GetNumSections();
		if (NumSections > 0)
		{
			const int32 Selection = FMath::RandRange(0,NumSections - 1);
			FName SectionName = DeathMontage->GetSectionName(Selection);
			AnimInstance->Montage_Play(DeathMontage);
			AnimInstance->Montage_JumpToSection(SectionName,DeathMontage);
			
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if (HealthBarComponent)
			{
				HealthBarComponent->SetVisibility(false);
			}
			
			const float SectionTime = DeathMontage->GetSectionLength(Selection);
			GetWorldTimerManager().SetTimer(DeathTimer,this,&AEnemy::DeathEnd,SectionTime);
		}
	}
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	//TO DO: Implement combat logic when notice players
	if (EnemyState != EEnemyState::EES_Patrolling) return;
	if (Pawn->ActorHasTag(FName("Player")))
	{
		CombatTarget = Pawn;
		UE_LOG(LogTemp,Warning,TEXT("See Player"))
		if (InTargetRange(CombatTarget,CombatRadius))
		{
			UE_LOG(LogTemp,Warning,TEXT("In Combat Range, Start chasing"))
			EnemyState = EEnemyState::EES_Chasing;
			GetWorldTimerManager().ClearTimer(PatrolTimer);
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			MoveToTarget(CombatTarget);
		}
	}
}


double AEnemy::CalculateImpactAngle(const FVector& ImpactLocation)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactPoint_Horizontal = FVector(ImpactLocation.X,ImpactLocation.Y,GetActorLocation().Z);
	const FVector ToHit = (ImpactPoint_Horizontal - GetActorLocation()).GetSafeNormal();
	
	/* cosTheta = A dot B    
	 * Theta = acos ( cosTheta ) */
	double Theta = FMath::Acos(FVector::DotProduct(Forward,ToHit));
	Theta = FMath::RadiansToDegrees(Theta);
	
	// If Hit from right, Crossproduct points down ( left-hand rule )
	const FVector CrossProduct = FVector::CrossProduct(Forward,ToHit);
	if (CrossProduct.Z < 0.f)
	{
		Theta*= -1.f;
	}
	
	return Theta;
}

void AEnemy::DirectionalHitReact(double Theta)
{
	FName Section = HitReactSections.Back;
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = HitReactSections.Front;
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = HitReactSections.Left;
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = HitReactSections.Right;
	}
	PlayHitReactMontage(Section);
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget,CombatRadius) && EnemyState != EEnemyState::EES_Patrolling)
	{
		UE_LOG(LogTemp,Warning,TEXT("Chasing, but lose interest."))
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
	}else if (!InTargetRange(CombatTarget,AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		UE_LOG(LogTemp,Warning,TEXT("Chasing outside attack range"))
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}else if (InTargetRange(CombatTarget,AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
		UE_LOG(LogTemp,Warning,TEXT("Attacking inside attack range"))
		
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
	MoveRequest.SetAcceptanceRadius(10.f);
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


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
