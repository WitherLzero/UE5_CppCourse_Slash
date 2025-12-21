// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;

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
	 // Functions for Notify
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void SetupWeaponCollisionEnabled(ECollisionEnabled::Type CollisionType);
	
	void DirectionalHitReact(double Theta);
	double CalculateImpactAngle(const FVector& ImpactLocation);
	
	/*
	 *  Play Montage Functions
	 */
	virtual void PlayAttackMontage();
	virtual void PlayDeathMontage();
	void PlayHitReactMontage(const FName SectionName);
	
	/*
	 *  Components
	 */
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	
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
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly,category = Montage)
	FHitReactSections HitReactSections;
	
private:

public:
};
