// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "enemy_level_2.h"
#include "PawnBase.h"
#include "Kismet/KismetMathLibrary.h"

Aenemy_level_2::Aenemy_level_2()
{
	health = UKismetMathLibrary::RandomIntegerInRange(minHealth, maxHealth);
	// this actor's health will be chosen randomly picking random value between arbitrary boundraies specified from blueprint
}

void Aenemy_level_2::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	APawnBase* pawn = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (pawn) {
		FVector subsVector = GetActorLocation() - pawn->GetActorLocation();
		subsVector.Z = 0;
		SetActorRotation(subsVector.Rotation()); //this actor also can head up to player
	}
}