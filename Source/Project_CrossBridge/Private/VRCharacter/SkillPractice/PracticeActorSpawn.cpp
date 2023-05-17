// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/PracticeActorSpawn.h"

// Sets default values
APracticeActorSpawn::APracticeActorSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APracticeActorSpawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsBulletTest)
	{
		BulletTarget = GetWorld()->SpawnActor<ABulletTarget>(SpawnBulletTarget, GetActorLocation(), GetActorRotation());
	}
}

// Called every frame
void APracticeActorSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

