// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/VRComponent.h"

// Sets default values
AVRComponent::AVRComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetIsReplicated(true);

}

// Called when the game starts or when spawned
void AVRComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

