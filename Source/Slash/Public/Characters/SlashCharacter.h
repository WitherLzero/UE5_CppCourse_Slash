// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Framework/BaseCharacter.h"
#include "_Enums/CharacterTypes.h"
#include "SlashCharacter.generated.h"

class AItem;
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;

class UInputMappingContext;
class UInputAction;


UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;
	
	// Input Assets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* EchoMappingContext;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;

	// Input Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKeyPressed();
	virtual void Attack() override;

	/* Play Montage Functions */
	virtual void PlayAttackMontage() override;
	void PlayEquipMontage(FName SectionName);
	
	// Functions for Attack Notify
	virtual void AttackEnd() override;
	UFUNCTION(BlueprintCallable)
	void EnableCombo();
	UFUNCTION(BlueprintCallable)
	void DisableCombo();
	
	// Functions for Arm Notify
	UFUNCTION(BlueprintCallable)
	void Arm();
	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void FinishArming();
	
private:
	// Character States
	UPROPERTY(VisibleInstanceOnly)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly)
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	// Components
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere,Category=Groom)
	UGroomComponent* Hair;
	
	UPROPERTY(VisibleAnywhere,Category=Groom)	
	UGroomComponent* Eyebrows;
	
	// Interactables
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	void InteractWithItem();
	
	// Weapon
	void EquipWeapon();
	FORCEINLINE bool CanArm() const { return ActionState == EActionState::EAS_Unoccupied && 
			CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon; }
	FORCEINLINE bool CanDisarm() const { return ActionState == EActionState::EAS_Unoccupied &&
			CharacterState == ECharacterState::ECS_Equipped;}
	
	// Attack
	int32 AttackIndex = 0;
	bool bCanCombo = false;
	FVector2D LastInputAxis = FVector2D::ZeroVector;
	void RotateToInputDirection();
	
	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
public:
	// Inner inline functions
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState NewState) { CharacterState = NewState; }
	FORCEINLINE void SetOverlappingItem(AItem* NewItem) { OverlappingItem = NewItem; }
	FORCEINLINE void SetWeapon(AWeapon* Weapon) { EquippedWeapon = Weapon; }
	
	// Inner helper functions
	void SetupComponents();
};
