// copyright: none


#include "Items/Item.h"

#include "Components/SphereComponent.h"
#include "Slash/DebugMacro.h"

// Sets default values
AItem::AItem()
	: RunningTime(0.f), Amplitude(0.5f), TimeConstant(2.f) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

}

float AItem::TransformedSine() const
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCosine() const
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		const FString ThisActorName = GetName();
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Item Overlap Message From: %s"), *ThisActorName));
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, TEXT("Item::OnSphereOverlap hasn't been Overridden in child class!"));
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		const FString ThisActorName = GetName();
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Item End Overlap Message From: %s"), *ThisActorName));
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Red, TEXT("Item::OnSphereEndOverlap hasn't been Overridden in child class!"));
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay() 
{
	Super::BeginPlay();
	
	// Bind the callback to the delegate
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

}

