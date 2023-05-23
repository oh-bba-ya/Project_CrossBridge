// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/TrashSpawningPool.h"
#include "Components/BoxComponent.h"
#include "Objects/Trash.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

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
	BoxComp->SetBoxExtent(TrashSpawningPoolSize);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BoxComp);
	TrashRangeEffect1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrashRangeEffect1"));
	TrashRangeEffect1->SetupAttachment(RootComponent);
	TrashRangeEffect1->SetRelativeLocation(FVector(TrashSpawningPoolSize.X, TrashSpawningPoolSize.Y, 0));
	TrashRangeEffect2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrashRangeEffect2"));
	TrashRangeEffect2->SetupAttachment(RootComponent);
	TrashRangeEffect2->SetRelativeLocation(FVector(-TrashSpawningPoolSize.X, TrashSpawningPoolSize.Y, 0));
	TrashRangeEffect3 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrashRangeEffect3"));
	TrashRangeEffect3->SetupAttachment(RootComponent);
	TrashRangeEffect3->SetRelativeLocation(FVector(TrashSpawningPoolSize.X, -TrashSpawningPoolSize.Y, 0));
	TrashRangeEffect4 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrashRangeEffect4"));
	TrashRangeEffect4->SetupAttachment(RootComponent);
	TrashRangeEffect4->SetRelativeLocation(FVector(-TrashSpawningPoolSize.X, -TrashSpawningPoolSize.Y, 0));
	TrashRangeDeactivate();
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
	if (IsRangeActivate)
	{
		TrashRangeActivate(TrashRangeEffect1);
		TrashRangeActivate(TrashRangeEffect2);
		TrashRangeActivate(TrashRangeEffect3);
		TrashRangeActivate(TrashRangeEffect4);
	}

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
			float RangeX = UKismetMathLibrary::RandomFloatInRange(GetActorLocation().X + TrashSpawningPoolSize.X, GetActorLocation().X - TrashSpawningPoolSize.X);
			float RangeY = UKismetMathLibrary::RandomFloatInRange(GetActorLocation().Y + TrashSpawningPoolSize.Y, GetActorLocation().Y - TrashSpawningPoolSize.Y);
			TrashArr[ArrIndex]->ServerActivate(FVector(RangeX, RangeY, GetActorLocation().Z));
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

void ATrashSpawningPool::ServerDeactivate_Implementation()
{
	IsActivate = false;
	SetActorLocation(FVector(0, 0, -1000));
}

void ATrashSpawningPool::TrashRangeActivate(UNiagaraComponent* TrashRangeEffect)
{
	TrashRangeEffect->SetVisibility(true);
	FVector TrashRangeStart = TrashRangeEffect->GetComponentLocation();
	FVector TrashRangeEnd = TrashRangeStart - TrashRangeEffect->GetUpVector() * 100000;

	FHitResult HitInfo;
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitInfo, TrashRangeStart, TrashRangeEnd, ECC_Visibility, Param);
	if (IsHit)
	{
		TrashRangeEnd = HitInfo.Location;
	}
	TArray<FVector> TrashRangeTraces;
	TrashRangeTraces.Add(TrashRangeStart);
	TrashRangeTraces.Add(TrashRangeEnd);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(TrashRangeEffect, FName(TEXT("User.PointArray")), TrashRangeTraces);
}
void ATrashSpawningPool::TrashRangeDeactivate()
{
	TrashRangeEffect1->SetVisibility(false);
	TrashRangeEffect2->SetVisibility(false);
	TrashRangeEffect3->SetVisibility(false);
	TrashRangeEffect4->SetVisibility(false);
}