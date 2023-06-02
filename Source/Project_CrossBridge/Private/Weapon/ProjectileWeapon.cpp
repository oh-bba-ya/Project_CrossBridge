// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeapon.h"

#include "BaseCharacterController.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Character/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HUD/WeaponHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Projectile.h"

// Sets default values
AProjectileWeapon::AProjectileWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("WeaponPreset"));
	BoxComponent->SetSimulatePhysics(true);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileWeapon::OnBoxComponentBeingOverlap);
}

// Called every frame
void AProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AProjectileWeapon::OnBoxComponentBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);

		if(player != nullptr && player->GetOwningWeapon() == nullptr && player->GetHomingItem() == nullptr)
		{
			if(OwnerCharacter != nullptr)
			{
				OwnerCharacter->SetEquip(false);
				OwnerCharacter->SetWeapon(nullptr);	
			}
			PickUp(player);
		}
	}
}




void AProjectileWeapon::Fire(ABaseCharacter* player, const FVector hitTarget)
{
	if(player != nullptr)
	{
		Server_Fire(player,hitTarget);
	}
}




void AProjectileWeapon::Server_Fire_Implementation(ABaseCharacter* player, const FVector hitTarget)
{
	
	const USkeletalMeshSocket* muzzleSocket = MeshComponent->GetSocketByName(FName("MuzzleFlash"));
	if(muzzleSocket)
	{
		FTransform SocketTransform = muzzleSocket->GetSocketTransform(MeshComponent);
		FVector ToTarget = hitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
		if(projectileFactory)
		{
			bFireDelay = false;
			FTimerHandle fireDelayHandle;
			GetWorldTimerManager().SetTimer(fireDelayHandle, FTimerDelegate::CreateLambda([&](){bFireDelay= true;}), fireDelayTime,false);
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(projectileFactory, SocketTransform.GetLocation(),TargetRotation);
			if(Projectile != nullptr)
			{
				Projectile->SetOwner(player);
				Multicast_Fire();

			}
		}
	}
}


void AProjectileWeapon::Multicast_Fire_Implementation()
{
	if(muzzleFlash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(muzzleFlash,MeshComponent,FName("MuzzleFlash"),MeshComponent->GetSocketLocation(FName("MuzzleFlash")),MeshComponent->GetSocketRotation(FName("MuzzleFlash")),EAttachLocation::KeepWorldPosition,true);
	}
}

/** Drop Weapon */
#pragma region DropWeapon()
void AProjectileWeapon::DropWeapon(ABaseCharacter* player)
{
	if(player !=nullptr)
	{
		Server_DropWeapon(player);
	}
}

void AProjectileWeapon::Server_DropWeapon_Implementation(ABaseCharacter* player)
{
	MultiCast_DropWeapon(player);
	SetOwner(nullptr);
}

void AProjectileWeapon::MultiCast_DropWeapon_Implementation(ABaseCharacter* player)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetSimulatePhysics(true);
	player->SetWeapon(nullptr);
	OwnerCharacter = nullptr;
	bEquippedWeapon = false;
	player->SetEquip(false);
}
#pragma endregion

/** Pickup Weapon */
#pragma region PickUp()
void AProjectileWeapon::PickUp(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		Server_PickupWeapon(player);
	}
}


void AProjectileWeapon::Multicast_PickupWeapon_Implementation(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		player->SetWeapon(this);
		BoxComponent->SetSimulatePhysics(false);
		AttachToComponent(player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		OwnerCharacter = player;
		bEquippedWeapon = true;
		player->SetEquip(true);
	}
}

void AProjectileWeapon::Server_PickupWeapon_Implementation(ABaseCharacter* player)
{
	Multicast_PickupWeapon(player);
	SetOwner(player);

}
#pragma endregion




// 서버에 복제 등록하기 위한 함수
void AProjectileWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileWeapon,Ammo);
	DOREPLIFETIME(AProjectileWeapon,Damage);
	DOREPLIFETIME(AProjectileWeapon,bFireDelay);
}


