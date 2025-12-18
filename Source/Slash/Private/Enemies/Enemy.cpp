// copyright: none


#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
	
}

void AEnemy::GetHit_Implementation(const FVector& ImpactLocation)
{
	double Theta = CalculateImpactAngle(ImpactLocation);

	DirectionalHitReact(Theta);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,HitSound,ImpactLocation);
	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,HitParticles,ImpactLocation);
	}
	
	
	DRAW_SPHERE_Duration(ImpactLocation,5.f)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1,5.f,FColor::Red,FString::Printf(TEXT("Theta: %.2f"),Theta));
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
	
	/* Temporal debugs */
	UKismetSystemLibrary::DrawDebugArrow(this,GetActorLocation(),GetActorLocation()+Forward*50.f,
									5.f,FColor::Red,5.f);
	UKismetSystemLibrary::DrawDebugArrow(this,GetActorLocation(),ImpactPoint_Horizontal,
										5.f,FColor::Green,5.f);
	UKismetSystemLibrary::DrawDebugArrow(this,GetActorLocation(),GetActorLocation()+CrossProduct*50.f,
										5.f,FColor::Blue,5.f);
	
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

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}