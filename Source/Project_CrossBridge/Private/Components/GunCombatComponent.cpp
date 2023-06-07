// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GunCombatComponent.h"

#include "Character/CBelica.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Projectile.h"

// Sets default values for this component's properties
UGunCombatComponent::UGunCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
void UGunCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
}


/** HitTarget */
#pragma region HitTarget

void UGunCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewprotSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewprotSize);
	}

	FVector2d CrosshairLocation(ViewprotSize.X / 2.f , ViewprotSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
		);

	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * TraceLength;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
			);

		if(!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		else
		{
			HitTarget = TraceHitResult.ImpactPoint;
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12,
				FColor::Red
				);
		}
	}
	
}
#pragma endregion 


/** Fire*/
#pragma region Fire
void UGunCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if(Character && bFireButtonPressed)
	{
		Fire();
	}
}

void UGunCombatComponent::BasicAttack()
{
	if(Character != nullptr)
	{
		const USkeletalMeshSocket* barrelSocket = Character->GetMesh()->GetSocketByName(FName("BarrelSocket"));
		if(barrelSocket)
		{
			// 소환을 시킨 주인공
			APawn* InstigatorPawn = Cast<APawn>(GetOwner());
			FTransform SocketTransform = barrelSocket->GetSocketTransform(Character->GetMesh());
			FVector ToTarget = HitTarget - SocketTransform.GetLocation();
			FRotator TargetRotation = ToTarget.Rotation();
			if(ProjectileFactory && InstigatorPawn)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = InstigatorPawn;
				GetWorld()->SpawnActor<AProjectile>(ProjectileFactory,SocketTransform.GetLocation(),TargetRotation,SpawnParams);
				//UE_LOG(LogTemp,Warning,TEXT("Projectile Spawn"));
			}
		}
	}
}

void UGunCombatComponent::Fire()
{
	Server_Fire();
}

void UGunCombatComponent::Server_Fire()
{
	Multicast_Fire();
}

void UGunCombatComponent::Multicast_Fire()
{
	BasicAttack();
}


#pragma endregion

/** Aiming */
#pragma region Aiming
void UGunCombatComponent::SetAiming(bool bisAiming)
{
	bAiming = bisAiming;
	ServerSetAiming(bisAiming);
}

void UGunCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	MulitCastSetAiming(bIsAiming);
}

void UGunCombatComponent::MulitCastSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}
#pragma endregion 


void UGunCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGunCombatComponent, bAiming);
}


