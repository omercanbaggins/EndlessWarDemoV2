// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlappingObjects.h"
//#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Components/SceneComponent.h"
#include "PawnBase.h"
#include "Kismet/KismetSystemLibrary.h"



// Sets default values
AOverlappingObjects::AOverlappingObjects()
{
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("mesh3d"));
	collisionSimple = CreateDefaultSubobject<USphereComponent>(FName("collision"));
	scene = CreateDefaultSubobject<USceneComponent>(FName("scenecomp"));
	SetRootComponent(scene);
	collisionSimple->SetupAttachment(GetRootComponent());
	mesh->SetupAttachment(RootComponent);
}

void AOverlappingObjects::BeginPlay()
{
	FTimerHandle spawnTimer2;

	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(spawnTimer2, this, &AOverlappingObjects::autoKill, killTime, false); //for killing old enemies to not consume unneccessit memory
}
void AOverlappingObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(-1* speedConstantforMoving* GetActorForwardVector()); //all enemies will move along this vector for static object constant can be arranged as 0
}
void AOverlappingObjects::Interact(){ // for play effects but i did not finished this functions 
	//UGameplayStatics::PlaySound2D(this, sound);
	//UGameplayStatics::SpawnEmitterAttached(effect, scene, FName("scene"), GetActorLocation(), GetActorRotation());
}

void AOverlappingObjects::autoKill() {
	Destroy();  //to gain memory and performance, after certain time all enemies will be destroyed 
}