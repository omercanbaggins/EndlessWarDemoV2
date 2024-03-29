// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OverlappingObjects.h"
#include "EnemyHitInterface.h"
#include "OverlapRespond.h"
#include "enemy_normal.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSWAR_API Aenemy_normal : public AOverlappingObjects,public IOverlapRespond,public IEnemyHitInterface
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	virtual void respondToOverlap(class AWeapon* wep) override;
	virtual void affectPlayer(class AWeapon* weaponRef) override;  //this will be called when objects are destructed by the player or overlapped functions called.
	void Tick(float DeltaTime) override;
};
