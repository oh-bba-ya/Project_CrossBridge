// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/HomingItem.h"

// Sets default values
AHomingItem::AHomingItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHomingItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHomingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

