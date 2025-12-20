// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "_Enums/CharacterTypes.h"
#include "Animation/AnimInstance.h"
#include "SlashAnimInstance.generated.h"

class UCharacterMovementComponent;
class ASlashCharacter;
/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY( BlueprintReadOnly)
	ASlashCharacter* SlashCharacter;
	
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovement;
	
	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float GroundSpeed;
	
	UPROPERTY( BlueprintReadOnly, Category=Movement)
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement | Character State" )
	ECharacterState CharacterState;
};
