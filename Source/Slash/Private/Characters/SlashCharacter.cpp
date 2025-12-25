// copyright: none


#include "Characters/SlashCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"

#include "Items/Item.h"
#include "Interfaces/Interactable.h"

#include "Animation/AnimMontage.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Items/Weapons/Weapon.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	SetupComponents();
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("Player"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(EchoMappingContext,0);
		}

		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
		{
			UIOverlay = PlayerHUD->GetPlayerOverlay();
		}
	}

	if (UIOverlay && Attributes)
	{
		UIOverlay->SetHealthPrecent(Attributes->GetHealthPercent());
		UIOverlay->SetStaminaPrecent(1.f);
		UIOverlay->SetCoinText(0);
		UIOverlay->SetSoulText(0);
	}
}

float ASlashCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered,this,&ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Started,this,&ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction,ETriggerEvent::Triggered,this,&ASlashCharacter::Attack);
	}
}

/*
 * <ABaseCharacter> Overrides
 */

void ASlashCharacter::Attack()
{
	if (!CanAttack()) return;
	
	// Case 1: If idle, start the first attack
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		AttackIndex = 0;
		
		RotateToInputDirection();
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	// Case 2: If already attacking and within valid combo window (Instant Transition)
	else if (ActionState == EActionState::EAS_Attacking && bCanCombo)
	{
		bCanCombo = false; // Close window immediately to prevent double trigger
		AttackIndex++;
		// Reset combo loop if exceeding max sections
		if (AttackIndex > AttackMontageSections.Num())
		{
			AttackIndex = 0;
		}
		RotateToInputDirection();
		PlayAttackMontage();
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	bCanCombo = false; // ensure window is closed
	AttackIndex = 0;   // reset index
}

void ASlashCharacter::PlayAttackMontage()
{
	const FName SectionName = SelectMontageSection(AttackMontageSections,AttackIndex);
	PlayMontageSection(AttackMontage,SectionName);
}

bool ASlashCharacter::CanAttack() const
{
	return CharacterState != ECharacterState::ECS_Unequipped &&
		AttackMontageSections.Num() > 0;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactLocation, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionState = EActionState::EAS_HitReacting;
}

/*
 * Input Handlers
 */

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveAxis = Value.Get<FVector2D>();
	LastInputAxis = MoveAxis;
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (GetController())
	{
		const FRotator YawRotation(0.f,GetControlRotation().Yaw,0.f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward,MoveAxis.Y);
		
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right,MoveAxis.X);
	}
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxis.X);
		AddControllerPitchInput(LookAxis.Y);
	}
}

void ASlashCharacter::Jump()
{
	Super::Jump();
}

void ASlashCharacter::EKeyPressed()
{
	if (OverlappingItem)
	{
		InteractWithItem();
	}
	else
	{
		EquipWeapon();
	}
}

/*
 * Combat Handlers
 */

void ASlashCharacter::EquipWeapon()
{
	if (CanDisarm())
	{
		PlayEquipMontage(FName("Disarm"));
		CharacterState = ECharacterState::ECS_Unequipped;
		ActionState = EActionState::EAS_Arming;
	}else if (CanArm())
	{
		PlayEquipMontage(FName("Arm"));
		CharacterState = ECharacterState::ECS_Equipped;
		ActionState = EActionState::EAS_Arming;
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

/*
 * Anim Notifies
 */

void ASlashCharacter::EnableCombo()
{
	bCanCombo = true;
}

void ASlashCharacter::DisableCombo()
{
	bCanCombo = false;
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(),FName("RightHandSocket"));
	}
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(),FName("SpineSocket"));
	}
}

void ASlashCharacter::FinishArming()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

/*
 * Internal Helpers
 */

void ASlashCharacter::SetupComponents()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
	
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = TEXT("head");
	
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = TEXT("head");
}

void ASlashCharacter::InteractWithItem()
{
	IInteractable* Interface = Cast<IInteractable>(OverlappingItem);
	if (Interface)
	{
		Interface->Interact(this);
		OverlappingItem = nullptr;
	}	
}

void ASlashCharacter::RotateToInputDirection()
{
	if (LastInputAxis.IsNearlyZero()) return;

	if (GetController())
	{
		const FRotator YawRotation(0.f,GetControlRotation().Yaw,0.f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		const FVector TargetDirection = (Forward*LastInputAxis.Y + Right*LastInputAxis.X).GetSafeNormal();
	
		if (!TargetDirection.IsNearlyZero())
		{
			SetActorRotation(TargetDirection.Rotation());	
			LastInputAxis = FVector2D::ZeroVector;
		}
	}
}
