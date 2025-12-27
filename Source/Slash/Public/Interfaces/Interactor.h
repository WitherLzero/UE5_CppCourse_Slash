// copyright: none

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactor.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IInteractor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetOverlappingItem(class AItem* Item) = 0;
	virtual void OnEquip() = 0;
	virtual void PickupSouls(class ASoul* Soul) = 0;
};
