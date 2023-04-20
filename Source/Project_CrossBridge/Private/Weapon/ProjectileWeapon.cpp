// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeapon.h"

#include "BaseCharacterController.h"
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


	if(bEquippedWeapon && OwnerCharacter != nullptr &&OwnerCharacter->IsLocallyControlled())
	{
		TraceUnderCosshairs(HitResult);
	}

	SetHUDCrosshairs(DeltaTime);
}

void AProjectileWeapon::OnBoxComponentBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);

		if(player != nullptr && player->GetOwningWeapon() == nullptr)
		{
			if(OwnerCharacter != nullptr)
			{
				OwnerCharacter->SetWeapon(nullptr);	
			}
			PickUp(player);
			UE_LOG(LogTemp,Warning,TEXT("Collision Weapon"));
		}
	}
}




void AProjectileWeapon::Fire(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		Server_Fire(player,HitTarget);
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
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(projectileFactory, SocketTransform.GetLocation(),TargetRotation);
			Projectile->SetOwner(player);
		}
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
	}
}

void AProjectileWeapon::Server_PickupWeapon_Implementation(ABaseCharacter* player)
{
	Multicast_PickupWeapon(player);
	SetOwner(player);

}
#pragma endregion


/** Trace Crosshair */
#pragma region Trace Crosshair
void AProjectileWeapon::TraceUnderCosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation = ViewportSize * 0.5f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// 2D 스크린 좌표를 3D 월드 좌표로 변환
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
		);

	// 변환 작업 성공
	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = CrosshairWorldDirection * TraceLength;

		// 자기자신은 충돌에서 제외
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

		// LineTrace 범위안에 감지되는 액터가 존재하지 않을시..
		if(!TraceHitResult.bBlockingHit)
		{
			UE_LOG(LogTemp,Warning, TEXT("Not Trace"));
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint,12.f,12,FColor::Green);
		}
		else  // 범위안에 감지되는 액터가 존재한다면..
		{
			HitTarget = TraceHitResult.ImpactPoint;
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint,12.f,12,FColor::Red);
			UE_LOG(LogTemp,Warning,TEXT("Hit Name : %s"),*TraceHitResult.GetActor()->GetName());
		}
	}
	
}

void AProjectileWeapon::SetHUDCrosshairs(float DeletaTime)
{
	if(OwnerCharacter == nullptr || OwnerCharacter->Controller == nullptr)
	{
		return ;
	}

	Controller = Controller == nullptr ? Cast<ABaseCharacterController>(OwnerCharacter->Controller) : Controller;

	if(Controller)
	{
		HUD = HUD == nullptr ? Cast<AWeaponHUD>(Controller->GetHUD()) : HUD;

		if(HUD)
		{
			FHUDStruct HudStruct;
			HudStruct.CrosshairCenter = CrosshairsCenter;
			HudStruct.CrosshairRight = CrosshairsRight;
			HudStruct.CrosshairLeft = CrosshairsLeft;
			HudStruct.CrosshairBottom = CrosshairsBottom;
			HudStruct.CrosshairTop = CrosshairsTop;
			HUD->SetHUDStruct(HudStruct);
			bDisplayCrosshair = true;
		}
	}
}
#pragma endregion 

// 서버에 복제 등록하기 위한 함수
void AProjectileWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileWeapon,Ammo);
	DOREPLIFETIME(AProjectileWeapon,fireDelay);
	DOREPLIFETIME(AProjectileWeapon,Damage);
}


