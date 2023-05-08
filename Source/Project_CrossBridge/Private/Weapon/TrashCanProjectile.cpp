// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TrashCanProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATrashCanProjectile::ATrashCanProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionProfileName(FName("BulletPreset"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	// 이동방향으로 회전..
	MovementComponent->bRotationFollowsVelocity =true;

	

}

// Called when the game starts or when spawned
void ATrashCanProjectile::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ATrashCanProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

