// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Cannon.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Objects/VRCore.h"
#include "PickupItem/HomingItem.h"
#include "Weapon/HomingProjectile.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
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
	if(p!=nullptr)
	{
		p->mycanon = this;
	}
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

	if(HommingAmmo > 0)
	{
		if(p !=nullptr)
		{
			Server_HomingFire(p);
		}
	}
	
}



void ACannon::Server_HomingFire_Implementation(class ABaseCharacter* p)
{
	if(bFireDelay)
	{
		
		bFireDelay = false;
		FTimerHandle fireDelayHandle;
		GetWorldTimerManager().SetTimer(fireDelayHandle, FTimerDelegate::CreateLambda([&](){bFireDelay= true;}), fireDelayTime,false);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = p;
		SpawnParameters.Owner= p;

		AHomingProjectile* homing = GetWorld()->SpawnActor<AHomingProjectile>(HomingFactory,Arrow->GetComponentLocation(),Arrow->GetComponentRotation(),SpawnParameters);

		if(homing != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, FString::Printf(TEXT("homingSpawn")));
			Multicast_HomingFire(p,homing);
			//testMulticast(homing);
			SubtractHominAmmo(1);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, FString::Printf(TEXT("Not HOming")));
		}

		
		
	}

	
}

void ACannon::Multicast_HomingFire_Implementation(ABaseCharacter* p, class AHomingProjectile* h)
{
	if(h!=nullptr)
	{
		if(core != nullptr)
		{
			if(muzzleEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),muzzleEffect,h->GetActorLocation());
			}

			if(FireSound)
			{
				if(FireAttenu)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound,GetActorLocation(),1,1,0,FireAttenu);
				}
			}
			
			h->MovementComponent->HomingTargetComponent = core->GetRootComponent();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, FString::Printf(TEXT("Client")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, FString::Printf(TEXT("not multicast homing")));
	}
}


void ACannon::testMulticast_Implementation(AHomingProjectile* h)
{
	if(h!=nullptr)
	{
		if(core != nullptr)
		{
			if(muzzleEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),muzzleEffect,h->GetActorLocation());
			}

			if(FireSound)
			{
				if(FireAttenu)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound,GetActorLocation(),1,1,0,FireAttenu);
				}
			}
			
			h->MovementComponent->HomingTargetComponent = core->GetRootComponent();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, FString::Printf(TEXT("Client")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, FString::Printf(TEXT("not Test multicast homing")));
	}
}

void ACannon::SetHommingAmmo(int32 v)
{
	v = v >= 0 ? v : 0;
	HommingAmmo = v;
}

void ACannon::AddHomingAmmo(int32 v)
{
	HommingAmmo += v;
}

void ACannon::SubtractHominAmmo(int32 v)
{
	HommingAmmo -= v;
	HommingAmmo = HommingAmmo >= 0 ? HommingAmmo : 0;
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

void ACannon::Reload(AHomingItem* homingPro)
{
	if(homingPro != nullptr)
	{
		Server_Reload(homingPro);
	}
}


void ACannon::Server_Reload_Implementation(AHomingItem* homingPro)
{
	HommingAmmo += 1;
}

void ACannon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACannon, core);
	DOREPLIFETIME(ACannon, HommingAmmo);
	DOREPLIFETIME(ACannon, bIsFire);
	DOREPLIFETIME(ACannon, targetComp);
	DOREPLIFETIME(ACannon, tempHoming);

}


