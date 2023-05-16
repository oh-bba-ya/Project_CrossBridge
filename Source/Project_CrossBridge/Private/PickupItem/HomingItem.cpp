// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/HomingItem.h"

#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AHomingItem::AHomingItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("ItemPreset"));
	BoxComponent->SetSimulatePhysics(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	

}

// Called when the game starts or when spawned
void AHomingItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHomingItem::OnSphereOverlap);
	
}




// Called every frame
void AHomingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void AHomingItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);

		if(player != nullptr && player->GetOwningWeapon() == nullptr && player->GetHomingItem() == nullptr)
		{
			if(Ownerplayer != nullptr)
			{
				Ownerplayer->SetHomingItem(nullptr);
			}
			PickUp(player);
		}
	}
}

void AHomingItem::PickUp(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		Server_PickUp(player);
	}
}


void AHomingItem::Multicast_PickUp_Implementation(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		player->SetHomingItem(this);
		BoxComponent->SetSimulatePhysics(false);
		AttachToComponent(player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		Ownerplayer = player;
		player->SetEquip(true);
	}
}

void AHomingItem::Server_PickUp_Implementation(ABaseCharacter* player)
{
	SetOwner(player);
	Multicast_PickUp(player);
}


void AHomingItem::DropItem(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		Server_DropItem(player);
	}
}

void AHomingItem::MultiCast_DropItem_Implementation(ABaseCharacter* player)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetSimulatePhysics(true);
	player->SetHomingItem(nullptr);
	Ownerplayer = nullptr;
	player->SetEquip(false);
}

void AHomingItem::Server_DropItem_Implementation(ABaseCharacter* player)
{
	MultiCast_DropItem(player);
	SetOwner(nullptr);
}


void AHomingItem::UsingItem(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		Server_UsingItem(player);
	}
}

void AHomingItem::Multicast_UsingItem_Implementation(ABaseCharacter* player)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	player->SetHomingItem(nullptr);
	Ownerplayer = nullptr;
	player->SetEquip(false);
	Destroy();
}

void AHomingItem::Server_UsingItem_Implementation(ABaseCharacter* player)
{
	Multicast_UsingItem(player);
	SetOwner(nullptr);
	
}

