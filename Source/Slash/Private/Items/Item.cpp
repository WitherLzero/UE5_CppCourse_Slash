// copyright: none


#include "Items/Item.h"
#include "Slash/DebugMacro.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld(); // Get the world pointer that this actor is in
	
	SetActorLocation(FVector(0.f, 0.f,50.f));
	SetActorRotation(FRotator(0.f,45.f,0.f));
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	
	
	// use macro to draw 
	DRAW_SPHERE(Location);
	DRAW_VECTOR(Location, Location + Forward * 100.f);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

