// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

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
	
private:
	float RunningTime;
	
	// UPROPERTY(EditDefaultsOnly)： can see and edit variables in blueprints
	// UPROPERTY(EditInstanceOnly)： can see and edit variables on instances
	UPROPERTY(EditAnywhere) // can see and edit variables both in blueprints and on instances
	float Amplitude;
	UPROPERTY(EditAnywhere)
	float TimeConstant;

};
