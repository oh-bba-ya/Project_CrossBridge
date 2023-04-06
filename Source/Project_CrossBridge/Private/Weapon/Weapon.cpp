// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::BasicAttack(const FVector& HitTarget)
{
	
}

void AWeapon::LineTraceFire(const FTransform SocketTransform)
{
	FHitResult hitInfo;
	const FVector Start = SocketTransform.GetLocation();
	const FQuat Rotation = SocketTransform.GetRotation();
	const FVector RotationAxis = Rotation.GetAxisX();
	const FVector End = Start + RotationAxis * 50000.f;
	FCollisionQueryParams param;

	param.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hitInfo,Start,End,ECollisionChannel::ECC_Visibility,param);

	if(hitInfo.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red,false,0.2f);
		DrawDebugPoint(GetWorld(),hitInfo.Location,5.f,FColor::Red, false, 2.f);
	}
	
}

