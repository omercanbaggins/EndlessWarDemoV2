// Fill out your copyright notice in the Description page of Project Settings.


#include "spawner.h"
#include "OverlapRespond.h"
#include "OverlappingObjects.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PawnBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"


// Sets default values
Aspawner::Aspawner()
{
	PrimaryActorTick.bCanEverTick = true;
	meshFloor = CreateDefaultSubobject<UStaticMeshComponent>(FName("meshf"));
	arrow = CreateDefaultSubobject<UArrowComponent>(FName("arrow"));
	boxCol = CreateDefaultSubobject<UBoxComponent>(FName("collisionc"));
	scene = CreateDefaultSubobject<USceneComponent>(FName("rootScene"));

	SetRootComponent(scene);
	boxCol->SetupAttachment(RootComponent);
	arrow->SetupAttachment(RootComponent); // its forward vector represents heading way of the way and which direction path will be extended. also new path spawns from this arrow location
	meshFloor->SetupAttachment(RootComponent);
	enemyGroup.InsertDefaulted(0);
	boundryRight = CreateDefaultSubobject<USceneComponent>(FName("B_right"));
	boundryLeft = CreateDefaultSubobject<USceneComponent>(FName("B_left"));
	boundryRight->SetupAttachment(meshFloor);
	boundryLeft->SetupAttachment(meshFloor);


}
FTimerHandle spawnTimer;

void Aspawner::BeginPlay()
{
	Super::BeginPlay();
	boxCol->OnComponentBeginOverlap.AddDynamic(this, &Aspawner::overlap);
	boxCol->OnComponentEndOverlap.AddDynamic(this, &Aspawner::EndOverlap);
	//UKismetSystemLibrary::K2_SetTimer(this, "SpawnEnemy", 2.0f, true);
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &Aspawner::SpawnEnemy, constantForSpawnTime*1.0f, true, 0.1f);
}

void Aspawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FActorSpawnParameters params = FActorSpawnParameters::FActorSpawnParameters();


void Aspawner::SpawnEnemy() {
	int rangeX;
	int rangeY;

	FVector currentPlayerLoc;

	rangeX = UKismetMathLibrary::RandomIntegerInRange(1000,1350);
	rangeY = UKismetMathLibrary::RandomIntegerInRange(-225, 225);  //these random values will add current player location to create new random location to spawn enemy

	int randomNumberForEnemyGroup = UKismetMathLibrary::RandomIntegerInRange(0,enemyGroup.Num()-1);  // this is related to how many enemy type is exist
	                                                                           //tihs enemy group can be increased from blueprint to create different difficulties
	                                                                           

	if (UKismetMathLibrary::Abs_Int(rangeY) < 25) {  // i want to decrease probability of enemy spawning in front of the player, if Y is to low. It must be at least 50 
		//to get distance i am using abs operation  //common aproach is determining constant location to use prodecural generation but i wanted to use different way
		if (rangeY < 0) {
			rangeY = -50;
		}
		else {
			rangeY = 50;
		}
	}
	if (Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(this, 0))) {

		currentPlayerLoc = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(this, 0))->getLoc();
		currentPlayerLoc.X += rangeX;
		currentPlayerLoc.Y += rangeY;
		//we dont want to allow enemies escape from path and go out of the path.
		if (currentPlayerLoc.Y > boundryRight->GetComponentLocation().Y)
			currentPlayerLoc.Y = boundryRight->GetComponentLocation().Y;  // this boundraies based on location of scene components 

		else if (currentPlayerLoc.Y < boundryLeft->GetComponentLocation().Y) { // spawn location of enemies is determined according to player current location
			currentPlayerLoc.Y = boundryLeft->GetComponentLocation().Y;  // this will prevent to enemy spawn outside of the path
		}

		GetWorld()->SpawnActor<AOverlappingObjects>(enemyGroup[randomNumberForEnemyGroup], currentPlayerLoc, FRotator(0), params);
	}
	else {

	}
}
void Aspawner::spawn()    //spawns new enemy which inherits from AOverlappableObjects
{
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	GetWorld()->SpawnActor<Aspawner>(spawnclass, arrow->GetComponentLocation(),FRotator(0), params);

}
void Aspawner::overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

};

void Aspawner::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
	if (Cast<APawnBase>(OtherActor->GetRootComponent()->GetAttachmentRootActor())) {
		boxCol->DestroyComponent();
		Destroy(); //to avoid new path to overlap with this collision, Destroy() is called before the spawn()
		spawn();
		// old path will be romoved and new path will be created to extend endless path
	}
	else { ; }
};

