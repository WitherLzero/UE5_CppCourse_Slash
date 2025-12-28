// copyright: none


#include "Items/Pickups/Soul.h"
#include "Interfaces/Interactor.h"


ASoul::ASoul()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	IInteractor* Interactor = Cast<IInteractor>(OtherActor);
	if (Interactor)
	{
		Interactor->PickupSouls(this);
	}
	SpawnPickupEffect();
	PlayPickupSound();
	
	Destroy();
}
