// Fill out your copyright notice in the Description page of Project Settings.


#include "weaponSpawner.h"
#include "Weapon.h"
#include "PawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TextRenderComponent.h"

void AweaponSpawner::BeginPlay() {
	Super::BeginPlay();
	a = randomInt(0);
	
}
int AweaponSpawner::randomInt(int b) { 
	b = UKismetMathLibrary::RandomIntegerInRange(-1, 1);
	if (b == 0) {
		return randomInt(b); //if random int is 0 call function again until 1 or -1

	}
	else {
		return b;
	}
}

AWeapon* AweaponSpawner::spawn(FVector loc,APawnBase* p){
	if(p->wepArray.Num()>0)
		return GetWorld()->SpawnActor<AWeapon>(p->wepArray[0]->GetClass(), loc, FRotator(0));
	else { return NULL; }
	//spawn and return pointer of spawned weapon
}	

int AweaponSpawner::DestroyWeapon(APawnBase* playerRef){
	if (!(playerRef->wepArray.Num() <= 0)) {
		//GEngine->AddOnScreenDebugMessage(1, 0.3f, FColor::Blue, FString::FromInt(wepArray.Num()));
		AWeapon* weptoDestroy = Cast<AWeapon>(playerRef->wepArray.Last());  //this actor always removes last weapon while other enemy removes touched weapon
		weptoDestroy->DestroyEffect();
		playerRef->wepArray.RemoveSingle(playerRef->wepArray.Last());   //after destroying array must be updated
	}
	return playerRef->wepArray.Num(); //if new array num 0 game must be finished to check that we should return new length
}

void AweaponSpawner::respondToOverlap(AWeapon* wep)
{ //this actor either add new weapon or remove a weapon from player and this is decided randomly with rand function 
	APawnBase* playerRef;
	
	playerRef = Cast<APawnBase>(wep->GetRootComponent()->GetAttachmentRootActor()); 

	if (playerRef!=NULL) {
		TArray<AActor*>& arrayforWeapons = playerRef->wepArray;

		switch (a) {
		case -1:
			if (DestroyWeapon(playerRef) == 0) {
				playerRef->GetNetOwningPlayer()->GetPlayerController(GetWorld())->RestartLevel();
			}
			break;
		case 1:
			if (arrayforWeapons.Num() == 0) {}
			FAttachmentTransformRules rules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
			AWeapon* lastweapon = Cast<AWeapon>(arrayforWeapons[arrayforWeapons.Num() - 1]);
			FVector lastV = lastweapon->GetActorLocation();
			FVector newV = lastV;
			AWeapon* newweapon;
			newV.SetComponentForAxis(EAxis::Y, lastV.Y + 60);
			newweapon = spawn(newV, playerRef);
			newweapon->AttachToComponent(playerRef->RootForWeaponGroup, rules);
			arrayforWeapons.Add(newweapon);  //we should add new weapon into array to follow it
			break;
		}
	}
	Destroy();
}

