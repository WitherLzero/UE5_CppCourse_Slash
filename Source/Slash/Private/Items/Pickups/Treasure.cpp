// copyright: none


#include "Items/Pickups/Treasure.h"
#include "Interfaces/Interactor.h"


// Sets default values
ATreasure::ATreasure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	
}



void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractor* Interactor = Cast<IInteractor>(OtherActor);
	if (Interactor)
	{
		Interactor->PickupGold(this);
		PlayPickupSound();
		Destroy();
		
	}
	
	
}

// Called every frame
void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

