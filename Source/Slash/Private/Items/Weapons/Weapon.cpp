// copyright: none


#include "Items/Weapons/Weapon.h"

#include "Characters/SlashCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"


void AWeapon::SetupCollision()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);  
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn,ECollisionResponse::ECR_Ignore);
}

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	SetupCollision();
	
	BoxTraceStart = CreateDefaultSubobject<USphereComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USphereComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnBoxOverlap);	
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	FHitResult BoxHit;
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,End,
		FVector(5.f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Type::ForDuration,
		BoxHit,
		true);
	
	if (BoxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
	}
}

void AWeapon::Interact(ASlashCharacter* Caller)
{
	AttachMeshToSocket(Caller->GetMesh(),FName("RightHandSocket"));
	ItemState = EItemState::EIS_Equipped;
	Caller->SetWeapon(this);
	Caller->SetCharacterState(ECharacterState::ECS_Equipped);

	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,EquipSound,GetActorLocation());
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

