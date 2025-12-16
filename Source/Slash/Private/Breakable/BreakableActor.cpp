// copyright: none


#include "Breakable/BreakableActor.h"

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Pickups/Treasure.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactLocation)
{
	if (UWorld* World = GetWorld())
	{
		FVector Location = GetActorLocation();
		Location.Z += 70.f;
		World->SpawnActor<ATreasure>(TreasureClass,Location,GetActorRotation());
	}
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}