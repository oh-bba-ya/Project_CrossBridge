// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/Trash.h"
#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrash::ATrash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetSimulatePhysics(false);
	SetActorHiddenInGame(true);

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ATrash::OnOverlap);
}

// Called when the game starts or when spawned
void ATrash::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATrash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATrash::GetActivate()
{
	return IsActivate;
}

void ATrash::ServerActivate_Implementation(FVector SpawnLoc)
{
	SetActorLocation(SpawnLoc);
	FTimerHandle PhysicsSetTimer;
	GetWorldTimerManager().SetTimer(PhysicsSetTimer, this, &ATrash::ServerPhysicsSet, 0.1, false);
	IsActivate = true;
	SetActorHiddenInGame(false);
	FTimerHandle DeactivateTimer;
	GetWorldTimerManager().SetTimer(DeactivateTimer, this, &ATrash::ServerDeactivate, 3, false);
}

void ATrash::ServerDeactivate_Implementation()
{
	SetActorLocation(FVector(0, 0, -1000));
	MeshComp->SetSimulatePhysics(false);
	IsActivate = false;
	SetActorHiddenInGame(true);
}

void ATrash::ServerPhysicsSet_Implementation()
{
	MeshComp->SetSimulatePhysics(true);
	FTimerHandle TrashSoundTimer;
	GetWorldTimerManager().SetTimer(TrashSoundTimer, this, &ATrash::MulticastTrashSoundPlay, 1, false);
}


void ATrash::MulticastTrashSoundPlay_Implementation()
{
		UGameplayStatics::PlaySoundAtLocation(this, VRTrashCanSound, GetActorLocation());
}

void ATrash::OnOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (IsActivate)
	{
		class ABaseCharacter *Enemy = Cast<ABaseCharacter>(OtherActor);
		if (Enemy)
		{
			if (!(Enemy->IsVR))
			{
				Enemy->OnTakeDamage(10);
			}
		}
	}
	ServerDeactivate();
}
