// copyright: none


#include "Items/Weapons/Weapon.h"

#include "Framework/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/Interactor.h"
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

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	FHitResult BoxHit;
	
	if (IsFriendly(OtherActor)) return;
	
	ExecuteBoxTrace(BoxHit);
	
	if (BoxHit.GetActor())
	{ 
		FString ActorName = BoxHit.GetActor()->GetName();
		
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
			);
		ApplyField(BoxHit.ImpactPoint);
		ExecuteGetHit(BoxHit);
	}
}

void AWeapon::Equipped(ABaseCharacter* Caller, FName SocketName)
{
	AttachMeshToSocket(Caller->GetMesh(),SocketName);
	ItemState = EItemState::EIS_Equipped;
	this->SetOwner(Caller);
	this->SetInstigator(Caller);
	Caller->SetWeapon(this);
}

void AWeapon::Interact(ABaseCharacter* Caller)
{
	if (!Caller) return;
	Equipped(Caller,FName("RightHandSocket"));
	
	IInteractor* Interactor = Cast<IInteractor>(Caller);
	if (Interactor)
	{
		Interactor->OnEquip();
	}
	
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,EquipSound,GetActorLocation());
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void AWeapon::ExecuteBoxTrace(FHitResult& OutBoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,End,
		FVector(5.f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug? EDrawDebugTrace::ForDuration : EDrawDebugTrace::Type::None,
		OutBoxHit,
		true);
	
	IgnoreActors.AddUnique(OutBoxHit.GetActor());
}

void AWeapon::ExecuteGetHit(const FHitResult& BoxHit) const
{
	if (BoxHit.GetActor()->Implements<UHitInterface>())
	{
		IHitInterface::Execute_GetHit(BoxHit.GetActor(),BoxHit.ImpactPoint,GetOwner());
	}
}

bool AWeapon::IsFriendly(AActor* OtherActor) const
{
	if (!GetOwner() && !OtherActor) return false;
	
	const TArray<FName>& OwnerTags = GetOwner()->Tags;
	const TArray<FName>& OtherTags = OtherActor->Tags;
	
	for (const FName& Tag : OwnerTags)
	{
		if (OtherTags.Contains(Tag)) return true;
	}
	return false;
}


