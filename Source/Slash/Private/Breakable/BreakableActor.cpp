// copyright: none


#include "Breakable/BreakableActor.h"

#include "GeometryCollection/GeometryCollectionComponent.h"


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
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}