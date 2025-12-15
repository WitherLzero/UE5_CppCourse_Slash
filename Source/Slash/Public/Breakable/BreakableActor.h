// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	// Interfaces
	void GetHit_Implementation(const FVector& ImpactLocation) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere);
	UGeometryCollectionComponent* GeometryCollection;
public:
};
