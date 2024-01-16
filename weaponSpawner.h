// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OverlappingObjects.h"
#include "OverlapRespond.h"
#include "weaponSpawner.generated.h"


/**
 */
class AWeapon;
class APawnBase;
UCLASS()
class ENDLESSWAR_API AweaponSpawner : public AOverlappingObjects,public IOverlapRespond
{
	GENERATED_BODY()
private:

public:
	class UTextRenderComponent* textNewUnitNumber;

	void BeginPlay() override;
	int randomInt(int b);
	AWeapon* spawn(FVector loc, APawnBase* p);
	int DestroyWeapon(APawnBase* playerRef);
	//AWeapon* spawn(FVector loc);
	void respondToOverlap(class AWeapon* wep);
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TSubclassOf<AWeapon> weaponClassToSpawn;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)

	//TArray<AActor*> wepArray;
	UPROPERTY(BlueprintReadOnly)
	int a=0;
};
