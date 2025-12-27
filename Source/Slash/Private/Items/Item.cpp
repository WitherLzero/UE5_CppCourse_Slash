// copyright: none


#include "Items/Item.h"

#include "Interfaces/Interactor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

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

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	ItemEffect->SetupAttachment(GetRootComponent());
	
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
	IInteractor* Interactor = Cast<IInteractor>(OtherActor);
	if (Interactor)
	{
		Interactor->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractor* Interactor = Cast<IInteractor>(OtherActor);
	if (Interactor)
	{
		Interactor->SetOverlappingItem(nullptr);
	}
}

void AItem::AttachMeshToSocket(USceneComponent* Parent, FName SocketName)
{
	const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,true);
	ItemMesh->AttachToComponent(Parent,Rules,SocketName);
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

