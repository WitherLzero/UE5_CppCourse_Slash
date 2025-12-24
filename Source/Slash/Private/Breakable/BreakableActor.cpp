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

void ABreakableActor::GetHit_Implementation(const FVector& ImpactLocation, AActor* Hitter)
{
	if (UWorld* World = GetWorld(); World && TreasureClasses.Num()>0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 70.f;
		int32 Selection = FMath::RandRange(0,TreasureClasses.Num()-1);
		World->SpawnActor<ATreasure>(TreasureClasses[Selection],Location,GetActorRotation());
	}
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}