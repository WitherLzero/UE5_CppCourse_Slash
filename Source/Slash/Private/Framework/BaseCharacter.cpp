// copyright: none


#include "Framework/BaseCharacter.h"

#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"



ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InitializeMesh();
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter)
{
	if (Attributes->IsAlive())
	{
		double Theta = CalculateImpactAngle(Hitter->GetActorLocation());
		DirectionalHitReact(Theta);
	}
	else
	{
		Die();
	}
	
	PlayHitSound(ImpactLocation);
	SpawnHitParticles(ImpactLocation);
}

void ABaseCharacter::HandleDamage(float Damage)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(Damage);
	}
}

bool ABaseCharacter::CanAttack() const
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionType)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionType);
		EquippedWeapon->IgnoreActors.Empty();
	}
}



void ABaseCharacter::PlayHitSound(const FVector& Location) const
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,HitSound,Location);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& Location) const
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,HitParticles,Location);
	}
}

void ABaseCharacter::PlayAttackMontage()
{
}

void ABaseCharacter::PlayDeathMontage()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName,HitReactMontage);
	}
}

bool ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage && !SectionName.IsNone())
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName,Montage);
		return true;
	}
	return false;
}

float ABaseCharacter::GetMontageSectionDuration(UAnimMontage* Montage, const FName& SectionName) const
{
	const int32 Index = DeathMontage->GetSectionIndex(SectionName);
	return DeathMontage->GetSectionLength(Index);
}

FName ABaseCharacter::SelectMontageSection(const TArray<FName>& SectionNames, int32 Index) const
{
	if (SectionNames.IsValidIndex(Index))
	{
		return SectionNames[Index];
	}
	return FName();
}

FName ABaseCharacter::SelectRandomMontageSection(const TArray<FName>& SectionNames) const
{
	const int32 Num = SectionNames.Num();
	if (Num <= 0) return FName();
	const int32 Selection = FMath::RandRange(0,Num - 1);
	return SectionNames[Selection];
}

void ABaseCharacter::InitializeMesh() const
{
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABaseCharacter::DirectionalHitReact(double Theta)
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

double ABaseCharacter::CalculateImpactAngle(const FVector& ImpactLocation)
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


TArray<FString> ABaseCharacter::GetAttackMontageSectionNames() const
{
	return GetSectionNamesFromMontage(AttackMontage);
}

TArray<FString> ABaseCharacter::GetDeathMontageSectionNames() const
{
	return GetSectionNamesFromMontage(DeathMontage);
}

TArray<FString> ABaseCharacter::GetSectionNamesFromMontage(UAnimMontage* Montage) const
{
	TArray<FString> Names;
	if (Montage)
	{
		for (int32 i = 0; i < Montage->GetNumSections(); ++i)
		{
			Names.Add(Montage->GetSectionName(i).ToString());
		}
	}
	return Names;
}


