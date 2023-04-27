// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Cannon.h"

#include "Character/BaseCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Objects/VRCore.h"
#include "Weapon/HomingProjectile.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("WeaponPreset"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocationAndRotation(FVector(30.f,0.f,37.f), FRotator(50.f,0.f,0.f));

}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ACannon::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this,&ACannon::OnEndOverlap);

	core = Cast<AVRCore>(UGameplayStatics::GetActorOfClass(GetWorld(),AVRCore::StaticClass()));
	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannon::HomingFire()
{
	Server_HomingFire();
}

void ACannon::Server_HomingFire_Implementation()
{
	if(bIsFire)
	{
		AHomingProjectile* homing = GetWorld()->SpawnActor<AHomingProjectile>(HomingFactory,Arrow->GetComponentLocation(),Arrow->GetComponentRotation());
		UE_LOG(LogTemp,Warning,TEXT("Projectile Spawn"));
		if(homing != nullptr)
		{
			if(core != nullptr)
			{
				homing->MovementComponent->HomingTargetComponent = core->GetRootComponent();
				homing->SetOwner(player);
			}
		}
	}
}

void ACannon::MultiCast_HomingFire_Implementation()
{
	Server_HomingFire();
}

void ACannon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ABaseCharacter* PCPlayer = Cast<ABaseCharacter>(OtherActor);

		if(PCPlayer != nullptr)
		{
			if(!(PCPlayer->IsVR))
			{
				bIsFire = true;
				player = PCPlayer;
				HomingFire();
			}
		}
	}

}

void ACannon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(HasAuthority())
	{
		ABaseCharacter* PCPlayer = Cast<ABaseCharacter>(OtherActor);

		if(PCPlayer != nullptr)
		{
			if(!(PCPlayer->IsVR))
			{
				bIsFire = false;
				
			}
		}
	}

}

void ACannon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACannon, core);
	DOREPLIFETIME(ACannon, bIsFire);

}


