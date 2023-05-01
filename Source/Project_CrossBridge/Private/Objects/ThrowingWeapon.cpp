// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/ThrowingWeapon.h"
#include "Components/BoxComponent.h"


// Sets default values
AThrowingWeapon::AThrowingWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(4, 1, 1));
	SetRootComponent(BoxComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetRelativeLocation(FVector(-3.5, 0, 0));
	MeshComp->SetRelativeRotation(FRotator(-90, 0, 0));
	MeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AThrowingWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AThrowingWeapon::BulletDestroy, 5.0f, false);
}

// Called every frame
void AThrowingWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * WeaponSpeed * DeltaTime);
}

void AThrowingWeapon::BulletDestroy()
{
	Destroy();
}
