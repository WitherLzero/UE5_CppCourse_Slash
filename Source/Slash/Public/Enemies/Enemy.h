// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"

#include "Slash/DebugMacro.h"
#include "Enemy.generated.h"

class UAnimMontage;

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
class SLASH_API AEnemy : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void GetHit_Implementation(const FVector& ImpactLocation) override;

	// Play Montage
	void PlayHitReactMontage(const FName SectionName);

private:
	// Animation montages
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditAnywhere,category = Sound)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere,category = VisualEffects)
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly,category = Montage)
	FHitReactSections HitReactSections;

	double CalculateImpactAngle(const FVector& ImpactLocation);
	void DirectionalHitReact(double Theta);

public:
};
