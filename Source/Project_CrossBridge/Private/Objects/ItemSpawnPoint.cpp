// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/ItemSpawnPoint.h"

#include "PickupItem/BasePickupItem.h"

// Sets default values
AItemSpawnPoint::AItemSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}

// Called when the game starts or when spawned
void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	StartSpawnItemTimer((AActor*)nullptr);
}

// Called every frame
void AItemSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AItemSpawnPoint::SpawnItem()
{
	int32 Num = Items.Num();

	if(Num > 0 )
	{
		int32 Selection = FMath::RandRange(0, Num -1);
		
		Item = GetWorld()->SpawnActor<ABasePickupItem>(Items[Selection], GetActorTransform());

		// 서버에서 동작, Item이 스폰되었다면..
		if(HasAuthority() && Item != nullptr)
		{
			// 스폰된 아이템 객체의 Destroy 함수에 바인딩..
			Item->OnDestroyed.AddDynamic(this, &AItemSpawnPoint::StartSpawnItemTimer);
		}
	}

}

void AItemSpawnPoint::SpawnItemTimerFinished()
{
	if(HasAuthority())
	{
		SpawnItem();
	}
}

void AItemSpawnPoint::StartSpawnItemTimer(AActor* DestroyedActor)
{
	UE_LOG(LogTemp,Warning,TEXT("StartTimer"));
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,
		this,
		&AItemSpawnPoint::SpawnItemTimerFinished,
		SpawnDelayTime,
		false);
}

