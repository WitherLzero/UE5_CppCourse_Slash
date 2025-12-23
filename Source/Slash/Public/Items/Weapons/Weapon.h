// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"


class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	void SetupCollision();
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;

	// Callbacks
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Interact
	virtual void Interact(ABaseCharacter* Caller) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyField(const FVector& FieldLocation);

private:
	void ExecuteBoxTrace(FHitResult& OutBoxHit);
	void ExecuteGetHit(const FHitResult& BoxHit) const;
	bool IsFriendly(AActor* OtherActor) const;
	
	UPROPERTY(EditInstanceOnly, Category = "Debug")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;
	
	UPROPERTY(VisibleAnywhere,Category = "Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	void Equipped(ABaseCharacter* Caller, FName SocketName);
	
	
};
