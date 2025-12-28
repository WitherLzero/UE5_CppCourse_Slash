// copyright: none
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Item.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped")
};

UCLASS()
class SLASH_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	void AttachMeshToSocket(USceneComponent* Parent, FName SocketName);
protected:
	virtual void BeginPlay() override;
	
	// Callback Functions
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()  
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Empty Interact
	virtual void Interact(ABaseCharacter* Caller) override {}
	
	// FXs
	void SpawnPickupEffect() const;
	void PlayPickupSound() const;
	
	
	// Math helper Functions
	UFUNCTION(BlueprintPure)
	float TransformedSine() const;

	UFUNCTION(BlueprintPure)
	float TransformedCosine() const;
	
	template<typename T>
	T Avg(T First, T Second);
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties")
	EItemState ItemState = EItemState::EIS_Hovering;
	
	// Component variables
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ItemEffect;
	
	// BlueprintReadOnly: can only get variables in event graph
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Paramaters") 
	float Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Paramaters")
	float TimeConstant;

	
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = "true")) 
	float RunningTime;
	
	/* FXs */
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;
	
	UPROPERTY(EditAnywhere,category = "Sounds")
	USoundBase* PickupSound;
};

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}


