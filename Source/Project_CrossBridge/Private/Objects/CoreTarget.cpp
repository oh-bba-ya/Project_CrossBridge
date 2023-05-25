
// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CoreTarget.h"

// Sets default values
ACoreTarget::ACoreTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

}

// Called when the game starts or when spawned
void ACoreTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoreTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

