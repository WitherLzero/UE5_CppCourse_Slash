// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"

class AWeapon;
class AItem;
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;

class UInputMappingContext;
class UInputAction;

class UAnimMontage;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
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
	void Attack();

	/* Play Montage Functions */
	void PlayAttackMontage();

	void PlayEquipMontage(FName SectionName);
	
	// Functions for Attack Notify
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void EnableCombo();

	UFUNCTION(BlueprintCallable)
	void DisableCombo();
	
private:
	// Character States
	UPROPERTY(VisibleInstanceOnly)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly)
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	int32 AttackIndex = 0;
	bool bCanCombo = false;
	
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
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapon;
	void EquipWeapon();

	FORCEINLINE bool CanArm() const { return ActionState == EActionState::EAS_Unoccupied && 
			CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon; }

	FORCEINLINE bool CanDisarm() const { return ActionState == EActionState::EAS_Unoccupied &&
			CharacterState == ECharacterState::ECS_Equipped;}
	
	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;
public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState NewState) { CharacterState = NewState; }
	FORCEINLINE void SetOverlappingItem(AItem* NewItem) { OverlappingItem = NewItem; }
	FORCEINLINE void SetWeapon(AWeapon* Weapon) { EquippedWeapon = Weapon; }
};
