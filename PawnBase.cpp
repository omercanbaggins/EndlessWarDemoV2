// Fill out your copyright notice in the Description page of Project Settings.
// omercandmrc

#include "PawnBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "OverlappingObjects.h"
#include "Components/BoxComponent.h"
#include "OverlapRespond.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

APawnBase::APawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
	RootForWeaponGroup = CreateDefaultSubobject<USceneComponent>(FName("soldierGroupAttachment"));

	//ChildfirstSoldier = CreateDefaultSubobject<UChildActorComponent>(FName("asker1"));
	cam = CreateDefaultSubobject<UCameraComponent>(FName("cam"));
	cmboom = CreateDefaultSubobject<USpringArmComponent>(FName("ip"));
	sphere = CreateDefaultSubobject<UBoxComponent>(FName("col"));

	SetRootComponent(sphere);
	//ChildfirstSoldier->SetupAttachment(RootComponent);
	RootForWeaponGroup->SetupAttachment(RootComponent);
	cmboom->SetupAttachment(RootForWeaponGroup);
	cam->SetupAttachment(cmboom);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

FVector APawnBase::getLoc() const
{
	return tempVec;
}

int APawnBase::returnIndexOfWeapon(AWeapon* wep)
{
	int i;
	//basic searching time complexity = O(n)
	for (i = 0; i < wepArray.Num(); i++) {
		if (wep == Cast<AWeapon>(wepArray[i])) {
			break;
		}
	}
	return i;

}
void APawnBase::slideWeaponsAfterKill(int RemovedItemIndex, FVector RemovedItemVector) {

	FVector LastVector = RemovedItemVector;  //location of removed weapon by the enemy
	// n-removedindex => Time comp O(n)
	for (int j = RemovedItemIndex+1; j < wepArray.Num(); j++) {
		// other weapons will change their location accordingly
		AWeapon* tempWeapon = Cast<AWeapon>(wepArray[j]);  // to hold old location of i th weapon.
		FVector tempVector = tempWeapon->GetActorLocation();
		tempWeapon->AddActorLocalOffset(LastVector- tempVector);
		LastVector = tempVector;  // this location will be used by the i+1 th weapon to find its new location
	}
	//when some weapons is killed by the enemy, locations will be ordered.

}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	StartVectorY = GetActorLocation().Y;
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	//ChildfirstSoldier->SetChildActorClass(childSoldierclass);
	//sphere->OnComponentBeginOverlap.AddDynamic(this, &APawnBase::OnComponentBeginOverlap);
	tempVec = FVector(0);
	//when game starts, first soldier is added to weapon array and attached to scene component to hold weaapons
	FActorSpawnParameters params;
	FAttachmentTransformRules tRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	AWeapon* firstWeapon =GetWorld()->SpawnActor<AWeapon>(childSoldierclass,GetActorLocation(), FRotator(0), params);
	wepArray.Add(firstWeapon);
	firstWeapon->AttachToComponent(RootForWeaponGroup, tRules);

}

void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddMovementInput(GetActorForwardVector()* multiplierForMovement);  // player always move straight 
	distance = distance+  multiplierForMovement;  // for score
	StartVectorY = GetActorLocation().Y;
	tempVec = GetActorLocation();
}

void APawnBase::slide(float V){

	if ((StartVectorY) > 200 && V > 0);
	else if ((StartVectorY) <-200 && V < 0);
	else AddMovementInput(GetActorRightVector() * V);  //this boundraies has been chosen for situation where player location start 0 point on the Y axis. 
														//if player is placed into another location that will cause to broke movement
													   // so if it is wanted to player placed arbitrary location, this code  should be updated according to location of constant actor such as path boundaries.
}

void APawnBase::ZoomInZoomOut(float V){
	if ((cmboom->TargetArmLength + 25 * V > 700) || (cmboom->TargetArmLength +25 * V < 200));
	else cmboom->TargetArmLength +=25*V;
}

void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("slide"), this, &APawnBase::slide); //for moving
	PlayerInputComponent->BindAxis(FName("Zoom"),  this, &APawnBase::ZoomInZoomOut); //camera Zoom in Zoom out
	PlayerInputComponent->BindAxis(FName("Rotate"), this, &APawnBase::rotatePlayer);
	PlayerInputComponent->BindAction(FName("Fire"),IE_Pressed,this,&APawnBase::Fire);

}

void APawnBase::Fire() {
	TArray<AActor*> weapons; 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), childSoldierclass, weapons);
	if (weapons.Num() > 0) {
		for (int i = 0; i < weapons.Num(); i++) {
			Cast<AWeapon>(weapons[i])->Fire();   // call fire functions of all weapons;
		}
	}
}
void APawnBase::rotatePlayer(float V) {
	if ((RootForWeaponGroup->GetComponentRotation().Yaw > 50 && V > 0)); //do nothing
	else if ((RootForWeaponGroup->GetComponentRotation().Yaw < -50 && V < 0)); // do nothing
	else RootForWeaponGroup->AddLocalRotation(FRotator(0, 3 * V, 0));  //  restrict player to rotate by more than 50 degree 
}
