// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Framework/BaseCharacter.h"
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
	
	/* <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/* </AActor> */

	
protected:
	/* <AActor> */
	virtual void BeginPlay() override;
	/* </AActor> */

	/* <ABaseCharacter> */
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void PlayAttackMontage() override;
	virtual bool CanAttack() const override;
	/* </ABaseCharacter> */
	
	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter) override;
	/* </IHitInterface> */


	/* Input Handlers */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKeyPressed();

	/* Combat Handlers */
	void EquipWeapon();
	void PlayEquipMontage(FName SectionName);
	
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

private:
	/* Internal Helpers */
	void SetupComponents();
	void InteractWithItem();
	void RotateToInputDirection();
	
	/* State Checks */
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
	
	UPROPERTY(VisibleInstanceOnly)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
	
	int32 AttackIndex = 0;
	bool bCanCombo = false;
	FVector2D LastInputAxis = FVector2D::ZeroVector;

public:
	/* Getters & Setters */
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState NewState) { CharacterState = NewState; }
	FORCEINLINE void SetOverlappingItem(AItem* NewItem) { OverlappingItem = NewItem; }

};
