// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enemy_normal.h"
#include "enemy_level_2.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSWAR_API Aenemy_level_2 : public Aenemy_normal
{
	GENERATED_BODY()
public:
	Aenemy_level_2();

	void Tick(float DeltaTime);
private:
	UPROPERTY(EditDefaultsonly)
	uint32 maxHealth;
	UPROPERTY(EditDefaultsonly)
	uint32 minHealth;
};
