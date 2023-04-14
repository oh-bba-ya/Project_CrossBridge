// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseGrabbableActor.h"

// Sets default values
ABaseGrabbableActor::ABaseGrabbableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetSimulatePhysics(true);
	
}

// Called when the game starts or when spawned
void ABaseGrabbableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGrabbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

