// Fill out your copyright notice in the Description page of Project Settings.


#include "enemy_normal.h"
#include "Weapon.h"
#include "PawnBase.h"
#include "Kismet/GameplayStatics.h"

void Aenemy_normal::BeginPlay() {
	Super::BeginPlay();

	health = Health;
}
void Aenemy_normal::respondToOverlap(AWeapon* wep)
{
	APawnBase* playerRef;
	Destroy();		
	playerRef = Cast<APawnBase>(wep->GetRootComponent()->GetAttachmentRootActor());
	if (!playerRef == NULL) {
		TArray<AActor*>& weaponArrayRef = playerRef->wepArray;   //we don't want to copy of array, we want to hold weapon number and remove some items when it needed we should use original array.
		//in order to avoid getallactorsofclass which consumes performance beacuse of for loop O(n), n-> located actors in world , I defined an array player locations and it adds new item when weapons are spawned.
		if (!(weaponArrayRef.Num() < 0)) {
			wep->DestroyEffect();
			FVector removedLoc = wep->GetActorLocation();
			int indexRemovedItem = playerRef->returnIndexOfWeapon(wep);
			playerRef->slideWeaponsAfterKill(indexRemovedItem, removedLoc);
			weaponArrayRef.RemoveSingle(wep);
			
			if (weaponArrayRef.Num() <= 0)
				playerRef->GetNetOwningPlayer()->GetPlayerController(GetWorld())->RestartLevel();
		}
	} 
}
void Aenemy_normal::affectPlayer(AWeapon* weaponRef)
{
	Destroy();
	APawnBase* player = Cast<APawnBase>(weaponRef->GetRootComponent()->GetAttachmentRootActor());  //weapons are attached to pawn's root, so this function will handle finding root actor
	if (player) {
		player->setPoint();  //this function adds 1 point 
	}
}
void Aenemy_normal::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	/*
	APawnBase* pawn = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (pawn) {
		FVector subsVector = GetActorLocation() - pawn->GetActorLocation();
		SetActorRotation(subsVector.Rotation());
	}*/
}