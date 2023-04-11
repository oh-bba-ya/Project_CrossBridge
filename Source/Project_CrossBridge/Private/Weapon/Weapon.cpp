// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "Character/CBelica.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionProfileName(TEXT("WeaponPreset"));
	/*
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	*/

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// 모든 무기는 서버가 관리한다.

	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);

	}
	
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACBelica* belica = Cast<ACBelica>(OtherActor);

	if(belica)
	{
		belica->SetOverlappingWeapon(this);
	}
	
	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACBelica* belica = Cast<ACBelica>(OtherActor);

	if(belica)
	{
		belica->SetOverlappingWeapon(nullptr);
	}
}






// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
	
	
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

