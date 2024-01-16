// Fill out your copyright notice in the Description page of Project Settings.


#include "enemy_Breakable.h"
#include "PawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"


/*Aenemy_Breakable::Aenemy_Breakable() {
	meshToDestruct = CreateDefaultSubobject<UDestructibleMComponent>(FName("destructible"));
	meshToDestruct-SetupAttachment(RootComponent)  i need to reserch how breakable components works 
}
*/ 

//this actor can offer player new weapons types also regardless of checking which or how many weapons touched to this actor, player is killed when overlaps.
//to get new weapon type player should kill this enemy before overlapping

Aenemy_Breakable::Aenemy_Breakable() {
	meshforW = CreateDefaultSubobject<UStaticMeshComponent>(FName("weaponToGive"));
	meshforW->SetupAttachment(RootComponent);
	meshforW->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void Aenemy_Breakable::BeginPlay() {

	Super::BeginPlay();
	int randomEnumIndex = UKismetMathLibrary::RandomIntegerInRange(1, 3);
	weaponToHold = (containedWeapon)randomEnumIndex;  //this enum holds weapon type. Since we want to assaign random weapon,Rand() method is so useful for that purpose 
	// to assaign random it i'm casting int to enumtype that i created
	health = Health; //one is come from interface other come from base enemy class this problem will be fixed
	if(weaponData)
		weaponStruct = weaponData->FindRow<FWeaponInformation>(UEnumFuncs::getEnumName(weaponToHold), "asd");
		if (weaponStruct){
			meshforW->SetStaticMesh(weaponStruct->weaponMesh);  

		}
	}

void Aenemy_Breakable::affectPlayer(AWeapon* weaponRef) //when this actor is killed this function is invoked in order to give player new weapon
{
	APawnBase* playerRef = Cast<APawnBase>(weaponRef->GetRootComponent()->GetAttachmentRootActor());
	weaponRef->GetClass();
	TArray<AActor*>& weaponArrayRef = playerRef->wepArray;  //O(n)  n depends on how many weapon is exists
	if (playerRef->wepArray.Num() > 0) {
		for (int i = 0; i < weaponArrayRef.Num() && weaponArrayRef[i]; i++) {
			Cast<AWeapon>(weaponArrayRef[i])->setWeaponType(UEnumFuncs::getEnumName(weaponToHold));
		}
	}
	Destroy();

}

void Aenemy_Breakable::respondToOverlap(AWeapon* wep)
{
	APawnBase* playerRef;

	playerRef = Cast<APawnBase>(wep->GetRootComponent()->GetAttachmentRootActor());

	if(playerRef)
		playerRef->GetNetOwningPlayer()->GetPlayerController(GetWorld())->RestartLevel();  
}
