// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/TrashSpawningPool.h"
#include "Components/BoxComponent.h"
#include "Objects/Trash.h"

#include "Math/Vector.h"

// Sets default values
ATrashSpawningPool::ATrashSpawningPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void ATrashSpawningPool::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < ArrSize; i++)
	{
		ServerSpawnTrash();
	}

	
}

// Called every frame
void ATrashSpawningPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActivate)
	{
		if (SpawningTimer == 0)
		{
			TrashSkillActivate();
		}
		SpawningTimer += DeltaTime;
		if (SpawningTimer > SpawningDelayTime)
		{
			SpawningTimer = 0;
		}
	}



}

void ATrashSpawningPool::TrashSkillActivate()
{
	while (1)
	{
		if (ArrIndex >= ArrSize)
		{
			ArrIndex %= ArrSize;
		}

		if (!TrashArr[ArrIndex]->IsActivate)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%s"), *GetActorLocation().ToString()));
			TrashArr[ArrIndex]->ServerActivate(GetActorLocation());
			ArrIndex = ++ArrIndex % ArrSize;
			break;
		}
		ArrIndex++;
	}
}

void ATrashSpawningPool::ServerSpawnTrash_Implementation()
{
	class ATrash* Trash = GetWorld()->SpawnActor<ATrash>(SpawnTrash, GetActorLocation(), GetActorRotation());
	TrashArr.Add(Trash);
}