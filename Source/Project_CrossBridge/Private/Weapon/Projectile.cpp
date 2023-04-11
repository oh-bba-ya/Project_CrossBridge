// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	// 이동방향으로 회전..
	MovementComponent->bRotationFollowsVelocity =true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

