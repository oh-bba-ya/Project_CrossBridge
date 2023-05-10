// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MaterialConverter.h"

#include "Character/BaseCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "PickupItem/HomingItem.h"

void AMaterialConverter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaterialConverter, GarbageCount);
	DOREPLIFETIME(AMaterialConverter, bIsMaking);
	
}

// Sets default values
AMaterialConverter::AMaterialConverter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GarbageCount = 0.f;
	myOwner = nullptr;
	bIsMaking = true;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionProfileName(FName("WeaponPreset"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(40.f,0,0));

	
}

// Called when the game starts or when spawned
void AMaterialConverter::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AMaterialConverter::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMaterialConverter::OnEndOverlap);

	
}

// Called every frame
void AMaterialConverter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




#pragma endregion 


void AMaterialConverter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AMaterialConverter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void AMaterialConverter::MakingHoming()
{
	if(GarbageCount >= RequireCount)
	{
		Server_MakingHoming();
	}
}

void AMaterialConverter::SetGarbageCount(float v)
{
	UE_LOG(LogTemp,Warning,TEXT("set Count : %.1f"),v);
	GarbageCount = v < 0 ? 0:v;
}

float AMaterialConverter::GetGarbageCount()
{
	return GarbageCount;
}

void AMaterialConverter::SaveGarbage(float v)
{
	Server_SaveGarbage(v);
}


void AMaterialConverter::Server_SaveGarbage_Implementation(float v)
{
	float garbage = GarbageCount + v;
	SetGarbageCount(garbage);
}


void AMaterialConverter::Server_MakingHoming_Implementation()
{
	if(bIsMaking)
	{
		bIsMaking = false;
		GarbageCount -= RequireCount;
		UE_LOG(LogTemp,Warning,TEXT("Garbage Count : %.1f"),GarbageCount);
		FTimerHandle makeDelayHandle;
		GetWorldTimerManager().SetTimer(makeDelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			bIsMaking= true;
			GetWorld()->SpawnActor<AHomingItem>(HomingItemFactory,Arrow->GetComponentLocation(),Arrow->GetComponentRotation());
		}), MakingTime,false);
	}
}


void AMaterialConverter::Entrance(ABaseCharacter* p)
{
	if(p != nullptr)
	{
		Server_Entrance(p);
	}
}

void AMaterialConverter::Server_Entrance_Implementation(ABaseCharacter* p)
{
	SetOwner(p);
	MultiCast_Entrance(p);
}

void AMaterialConverter::MultiCast_Entrance_Implementation(ABaseCharacter* p)
{
	if(p != nullptr)
	{
		p->SetConverter(this);
	}
}

void AMaterialConverter::Exit(ABaseCharacter* p)
{
	if(p != nullptr)
	{
		Server_Exit(p);
	}	
}

void AMaterialConverter::Server_Exit_Implementation(ABaseCharacter* p)
{
	SetOwner(nullptr);
	MultiCast_Exit(p);
}

void AMaterialConverter::MultiCast_Exit_Implementation(ABaseCharacter* p)
{
	p->SetConverter(nullptr);
}





