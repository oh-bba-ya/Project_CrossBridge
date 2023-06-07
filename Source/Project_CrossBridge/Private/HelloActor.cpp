// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloActor.h"

// Sets default values
AHelloActor::AHelloActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHelloActor::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Hello"));
}

// Called every frame
void AHelloActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

