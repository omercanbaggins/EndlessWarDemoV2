// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OverlappingObjects.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "EnemyHitInterface.h"
#include "OverlapRespond.h"
#include "Components/BoxComponent.h"


// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	rootScene = CreateDefaultSubobject<USceneComponent>(FName("root"));
	weaponFront = CreateDefaultSubobject<USphereComponent>(FName("namlu"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("weapon"));
	sphere = CreateDefaultSubobject<UBoxComponent>(FName("col"));
	SetRootComponent(rootScene);
	mesh->SetupAttachment(RootComponent);
	weaponFront->SetupAttachment(mesh); //to determine starting location for line trace, i am using this sphere's location
	timeForFiring = 1.0f;  // it is not used because i cancelled autofire

	mesh->SetNotifyRigidBodyCollision(true);
	mesh->BodyInstance.SetCollisionProfileName(FName("BlockAllDynamic"));
	sphere->SetupAttachment(mesh);


}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
		GEngine->AddOnScreenDebugMessage(11, 1.0f, FColor::Red, FString(GetOwner()->GetName()));
	//FTimerHandle handle;
	//GetWorldTimerManager().SetTimer(handle, this, &AWeapon::Fire, timeForFiring, true, timeForFiring); // i cancelled this feature to provide player more interactive gameplay 
	// timerForFiring variable is set from weapon datatable
	setWeaponType("auto");  //initial weapon type
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnComponentBeginOverlap); //bind overlap function to collision / it is actually note sphere it is a box, I named it wrong.
}


void AWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult) {

	IOverlapRespond* overlappedActor = Cast<IOverlapRespond>(OtherActor); //cast interface 
	if (overlappedActor) {
		overlappedActor->respondToOverlap(this); // call interface function, all enemies have own function body for overlap respond
	}
}

void AWeapon::Kill() {
	Destroy(); //after certain time this function will be invoked
}

void AWeapon::setWeaponType(FName rowname) {  // to reach weapon attributes from datatable
	if (table) {
		tableInstance = table->FindRow<FWeaponInformation>(rowname, "asd", true);

		if (tableInstance) {
			timeForFiring = tableInstance->firingTime; //if auto fire is activated this will be change fire time
			sound = tableInstance->firingSound;
			effect = tableInstance->firingParticle;
			mesh->SetStaticMesh(tableInstance->weaponMesh);
			//GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			//GetWorldTimerManager().SetTimer(timerhandle, this, &AWeapon::Fire, timeForFiring, true, timeForFiring);
			setDamage(tableInstance->damage); // all weapons have different damage constant,
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString("failed"), true);

	}
}
void AWeapon::setDamage(int d) 
{ 
	if (d>0)
	damage = d;
}


void AWeapon::Fire() {FHitResult hitresults;
FVector End;
IEnemyHitInterface* hitA;
	UGameplayStatics::PlaySound2D(this, sound);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, weaponFront->GetComponentLocation(), weaponFront->GetComponentRotation());

	End = weaponFront->GetComponentLocation() + mesh->GetForwardVector() * 1000;  //weapon will fire into its forward  this 1000 is used for adjust distance. It can be replaced with costant which can be rearanged from blueprint
	GetWorld()->LineTraceSingleByChannel(hitresults, weaponFront->GetComponentLocation(), End, ECollisionChannel::ECC_Camera);
	//DrawDebugLine(GetWorld(), weaponFront->GetComponentLocation(), End, FColor::Red, true, 0.2f);
	hitA = Cast<IEnemyHitInterface>(hitresults.GetActor());
	//another interface to cast

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, hitresults.ImpactPoint, FRotator(0));
	if (hitA) {
		if (hitA->getDamage(damage) <= 0) { //getDamage returns int which represents health of enemy
				hitA->affectPlayer(this); // this function is another "IHitInterface function" has instructions to perform gameplay affects which determine how player is affected by killing of enemy
		}
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AWeapon::DestroyEffect() {  // in order to give "animation looks", basic physics operation is applied.
	sphere->DestroyComponent();
	mesh->SetEnableGravity(true);
	mesh->SetSimulatePhysics(true);
	mesh->AddForce(FVector(8000, 8000, 80000.0f));
	GetWorldTimerManager().SetTimer(timerhandle, this, &AWeapon::Kill, 1.0f, false, 1.0f); //after specified time weapon will be destoryed

}