#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName="Unequipped"),
	ECS_Equipped UMETA(DisplayName="Equipped")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Dead UMETA(DisplayName = "Dead"),
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_HitReacting UMETA(DisplayName = "Hit Reaction"),
	EAS_Arming UMETA(DisplayName = "Arming")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_AttackWindow UMETA(DisplayName = "Attack Window"),
	EES_Attacking UMETA(DisplayName = "Attacking")
};