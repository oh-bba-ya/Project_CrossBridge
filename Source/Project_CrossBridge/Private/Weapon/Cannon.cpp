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



void ACannon::Entrance(class ABaseCharacter* p)
{
	if(p !=nullptr)
	{
		Server_Entrance(p);
	}
}



void ACannon::MultiCast_Entrance_Implementation(class ABaseCharacter* p)
{
	p->mycanon = this;
}

void ACannon::Server_Entrance_Implementation(class ABaseCharacter* p)
{
	SetOwner(p);
	MultiCast_Entrance(p);
}


void ACannon::Exit(class ABaseCharacter* p)
{
	if(p != nullptr)
	{
		Server_Exit(p);
	}
}

void ACannon::MultiCast_Exit_Implementation(class ABaseCharacter* p)
{
	p->mycanon = nullptr;
}

void ACannon::Server_Exit_Implementation(class ABaseCharacter* p)
{
	MultiCast_Exit(p);
	SetOwner(nullptr);
	
}



void ACannon::HomingFire(class ABaseCharacter* p)
{
	UE_LOG(LogTemp,Warning,TEXT("HOming Fire start"));
	Server_HomingFire(p);
}



void ACannon::Server_HomingFire_Implementation(class ABaseCharacter* p)
{
	AHomingProjectile* homing = GetWorld()->SpawnActor<AHomingProjectile>(HomingFactory,Arrow->GetComponentLocation(),Arrow->GetComponentRotation());
	UE_LOG(LogTemp,Warning,TEXT("Projectile Spawn"));
	Multicast_HomingFire(p,homing);
	homing->SetOwner(p);
}

void ACannon::Multicast_HomingFire_Implementation(ABaseCharacter* p, class AHomingProjectile* h)
{
	if(h!=nullptr)
	{
		if(core != nullptr)
		{
			h->MovementComponent->HomingTargetComponent = core->GetRootComponent();
		}
	}
}



void ACannon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);

		if(player != nullptr)
		{
			if(!(player->IsVR))
			{
				Entrance(player);
			}
		}
	}


}

void ACannon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(HasAuthority())
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);

		if(player != nullptr)
		{
			if(!(player->IsVR))
			{
				Exit(player);
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


