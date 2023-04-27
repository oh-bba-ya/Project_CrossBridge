// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/VRCore.h"

#include "Components/SphereComponent.h"

// Sets default values
AVRCore::AVRCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AVRCore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

