// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/HomingProjectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/VRCore.h"

// Sets default values
AHomingProjectile::AHomingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("BulletPreset"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	MovementComponent->bRotationFollowsVelocity =true;
	MovementComponent->bIsHomingProjectile = true;
	
}

// Called when the game starts or when spawned
void AHomingProjectile::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AHomingProjectile::OnOverlap);
	
}

void AHomingProjectile::Destroyed()
{
	FVector SapwnLoc = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplosionEffect,SapwnLoc,FRotator::ZeroRotator);
}

// Called every frame
void AHomingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHomingProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVRCore* core = Cast<AVRCore>(OtherActor);

	if(core != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Homing Destroy"));
		Destroy();
	}

	if(HasAuthority())
	{
		AVRCore* servercore = Cast<AVRCore>(OtherActor);
		if(servercore!=nullptr)
		{
			servercore->OnTakeDamage(Damage);
			servercore->ChangeColor();
		}
	}
}

