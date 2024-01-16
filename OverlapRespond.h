// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OverlapRespond.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOverlapRespond : public UInterface
{
	GENERATED_BODY()
};

class ENDLESSWAR_API IOverlapRespond
{
	GENERATED_BODY()
public:
	virtual void respondToOverlap(class AWeapon* wep) = 0; // function will be triggered inside beginoverlap function by the player pawn
	 //inherited to implement this interface.
};
