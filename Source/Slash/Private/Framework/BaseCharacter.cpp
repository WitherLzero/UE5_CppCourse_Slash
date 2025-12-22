// copyright: none


#include "Framework/BaseCharacter.h"

#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::HandleDamage(float Damage)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(Damage);
	}
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::SetupWeaponCollisionEnabled(ECollisionEnabled::Type CollisionType)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionType);
		EquippedWeapon->IgnoreActors.Empty();
	}
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

FName ABaseCharacter::SelectRandomMontageSection(const TArray<FName>& SectionNames) const
{
	const int32 Num = SectionNames.Num();
	if (Num <= 0) return FName();
	const int32 Selection = FMath::RandRange(0,Num - 1);
	return SectionNames[Selection];
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

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


