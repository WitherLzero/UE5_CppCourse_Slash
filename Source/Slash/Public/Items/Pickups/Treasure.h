// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

UCLASS()
class SLASH_API ATreasure : public AItem
{
	GENERATED_BODY()

public:
	ATreasure();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere,Category = "Treasure Properties")
	int32 Gold;
	
public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
