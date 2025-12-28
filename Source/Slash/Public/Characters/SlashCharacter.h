// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Framework/BaseCharacter.h"
#include "Interfaces/Interactor.h"
#include "SlashCharacter.generated.h"

class UPlayerOverlay;
class AItem;
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;

class UInputMappingContext;
class UInputAction;


UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IInteractor
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	
	/* <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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
	virtual bool CanAttack() const override;
	// Play Montage
	virtual void PlayAttackMontage() override;
	/* </ABaseCharacter> */
	
	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter) override;
	/* </IHitInterface> */
	
	/* <IInteractor> */
	virtual void SetOverlappingItem(AItem* NewItem) override;
	virtual void OnEquip() override;
	virtual void PickupSouls(ASoul* Soul) override;
	virtual void PickupGold(ATreasure* Treasure) override;
	/* </IInteractor> */


	/* Input Handlers */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dodge();
	virtual void Jump() override;
	void EKeyPressed();

	/* Combat Handlers */
	void EquipWeapon();
	
	/* Anim Notifies */
	UFUNCTION(BlueprintCallable)
	void EnableCombo();
	
	UFUNCTION(BlueprintCallable)
	void DisableCombo();
	
	UFUNCTION(BlueprintCallable)
	void Arm();
	
	UFUNCTION(BlueprintCallable)
	void Disarm();
	
	UFUNCTION(BlueprintCallable)
	void FinishArming();
	
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();
	
	/*
	 * Variables: Inputs
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* EchoMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DodgeAction;

private:
	/* Internal Helpers */
	void SetupComponents();
	void InteractWithItem();
	void RotateToInputDirection();
	// timer relevant
	void DeathEnd(); 
	// State Checks 
	FORCEINLINE bool IsUnoccupied() const { return ActionState == EActionState::EAS_Unoccupied; }
	FORCEINLINE bool CanDodge() const { return IsUnoccupied() && Attributes && Attributes->GetStamina() > DodgeCost;}
	FORCEINLINE bool CanArm() const { return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon; }
	FORCEINLINE bool CanDisarm() const { return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Equipped; }

	/*
	 * Variables: Components
	 */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Groom)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Groom)
	UGroomComponent* Eyebrows;

	/*
	 * Variables: State & Logic
	 */
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
	UPROPERTY()
	UPlayerOverlay* UIOverlay;
	
	UPROPERTY(VisibleInstanceOnly)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;
	
	/* 
	 * Combat
	 */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DodgeCost = 15.f;
	
	int32 AttackIndex = 0;
	bool bCanCombo = false;
	FVector2D LastInputAxis = FVector2D::ZeroVector;
	FTimerHandle DeathTimer;

	
public:
	/* Getters & Setters */
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState NewState) { CharacterState = NewState; }

};
