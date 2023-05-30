// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Freeze.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFreeze::AFreeze()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetBoxExtent(FVector(50.f,50.f,85.f));
	BoxComponent->SetCollisionProfileName(FName("FreezePreset"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MehsComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(0.7f,0.7f,0.7f));
	MeshComponent->SetRelativeLocation(FVector(0.f,0.f,-70.f));
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));
	
}

// Called when the game starts or when spawned
void AFreeze::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFreeze::Destroyed()
{
	if(breakEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),breakEffect,GetActorLocation());
	}

	if(DestroySound && DestroyAttenuation)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),DestroySound,GetActorLocation(),1,1,0,DestroyAttenuation);
	}
}

// Called every frame
void AFreeze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


