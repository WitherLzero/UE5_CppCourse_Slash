// copyright: none
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// BlueprintReadOnly: can only get variables in event graph
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Paramaters") 
	float Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Paramaters")
	float TimeConstant;

	// Math helper Functions
	UFUNCTION(BlueprintPure)
	float TransformedSine() const;

	UFUNCTION(BlueprintPure)
	float TransformedCosine() const;
	
	template<typename T>
	T Avg(T First, T Second);
	
	// Callback Functions
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()  
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = "true")) 
	float RunningTime;
};

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}


