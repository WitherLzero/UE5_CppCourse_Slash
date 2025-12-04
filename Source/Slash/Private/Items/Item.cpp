// copyright: none


#include "Items/Item.h"

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
	
	UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1,60.f,FColor::Cyan,TEXT("Item OnScreen Message!"));
	}

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UE_LOG(LogTemp, Warning, TEXT("Delta Time: %f"),DeltaTime);
	
	FString Name = GetName();
	FString Message = FString::Printf(TEXT("Item Name: %s"),*Name);  // use * to get the c-style array

	if(GEngine){
		GEngine->AddOnScreenDebugMessage(2,60.f,FColor::Blue,Message);
	}

}

