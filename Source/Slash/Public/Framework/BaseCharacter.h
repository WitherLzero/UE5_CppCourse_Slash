// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Components/AttributeComponent.h"
#include "Framework/Enums/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UMotionWarpingComponent;


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
class SLASH_API ABaseCharacter : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	
	/*
	 *  Combat
	 */
	virtual void Attack();
	virtual void Die();
	virtual void GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter) override;
	virtual void HandleDamage(float Damage);
	virtual void HandleDeathEnd(float AnimDuration);
	virtual void UpdateHealthUI() const;
	virtual bool CanAttack() const;
	 
	// Functions for Notify
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionType);
	UFUNCTION(BlueprintCallable)
	void UpdateWarpTargetLocation(FName WarpTargetName, float DistanceThreshold = 200.f);
	UFUNCTION(BlueprintCallable)
	void UpdateWarpTargetRotation(FName WarpTargetName);
	
	/*
	 *  FXs Functions
	 */
	void PlayHitSound(const FVector& Location) const;
	void SpawnHitParticles(const FVector& Location) const;
	
	
	/*
	 *  Play Montage Functions
	 */
	virtual void PlayAttackMontage();
	virtual void PlayDeathMontage();
	void PlayHitReactMontage(const FName& SectionName);
	
	bool PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const;
	float GetMontageSectionDuration(UAnimMontage* Montage, const FName& SectionName) const;
	FName SelectMontageSection(const TArray<FName>& SectionNames, int32 Index) const;
	FName SelectRandomMontageSection(const TArray<FName>& SectionNames) const ;
	
	/*
	 *  Helpers
	 */
	void SetMeshCollisionEnabled(ECollisionEnabled::Type CollisionType);
	bool IsAlive() const { return Attributes->IsAlive();}
	
	
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	AActor* CombatTarget;
	float WarpTargetDistance = 75.f;
	
	/*
	 *  Components
	 */
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	UPROPERTY(VisibleAnywhere)
	UMotionWarpingComponent* MotionWarping;
	
	/*
	 *  FXs
	 */
	UPROPERTY(EditAnywhere,category = Sound)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere,category = VisualEffects)
	UParticleSystem* HitParticles;	
	
	/*
	 * Owning Items
	 */
	UPROPERTY(VisibleInstanceOnly,Category=Weapon)
	AWeapon* EquippedWeapon;
	
	/*
	 *  Anim Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere,Category=Montage, meta=(GetOptions = "GetAttackMontageSectionNames"))
	TArray<FName> AttackMontageSections;
	
	UPROPERTY(EditAnywhere,Category=Montage, meta=(GetOptions = "GetDeathMontageSectionNames"))
	TArray<FName> DeathMontageSections;
	
	UPROPERTY(EditDefaultsOnly,category = Montage)
	FHitReactSections HitReactSections;
	
private:
	/*
 *  Helpers
 */
	void InitializeMesh() const;
	
	void DirectionalHitReact(double Theta);
	double CalculateImpactAngle(const FVector& ImpactLocation);
	
	FVector CalculateTranslationWarpTarget() const;
	FRotator CalculateRotationWarpTarget() const;
	
	UFUNCTION()
	TArray<FString> GetAttackMontageSectionNames() const;
	
	UFUNCTION()
	TArray<FString> GetDeathMontageSectionNames() const;
	
	TArray<FString> GetSectionNamesFromMontage(UAnimMontage* Montage) const;

public:
	FORCEINLINE void SetWeapon(AWeapon* Weapon) { EquippedWeapon = Weapon; }
	FORCEINLINE float GetHealthPercent() const { return Attributes->GetHealthPercent(); }
};
