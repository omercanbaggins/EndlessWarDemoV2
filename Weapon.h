// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "weaponStructs.h"
#include "Weapon.generated.h"

UCLASS()
class ENDLESSWAR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* sphere;

	virtual void Tick(float DeltaTime) override;
	void DestroyEffect(); //when fired when this weapon is killed
	UPROPERTY()
	FTimerHandle timerhandle;

	FWeaponInformation* tableInstance;
	UPROPERTY(EditDefaultsOnly)
	FName rowName;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;
	UPROPERTY(EditAnywhere)
	float timeForFiring;
	UPROPERTY(EditAnywhere)
	UParticleSystem* effect;
	UPROPERTY(EditAnywhere)
	USoundBase* sound;
	UPROPERTY(EditAnywhere)
	UDataTable* table;
	UPROPERTY(EditAnywhere)
	class USphereComponent* weaponFront;
	UPROPERTY(EditAnywhere)
	USceneComponent* rootScene;
	void Kill();
	void setWeaponType(FName rowname);
	void Fire();

	void setDamage(int d);
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	int damage = 1;
};
