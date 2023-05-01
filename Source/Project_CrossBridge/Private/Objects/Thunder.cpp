// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Thunder.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AThunder::AThunder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(MeshComp);
	BoxComp->SetBoxExtent(FVector(7, 7, 40));

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AThunder::OnOverlap);
}

// Called when the game starts or when spawned
void AThunder::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AThunder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (IsDirectionSet)
	{
		ThunderFloatingTime += DeltaTime;
		FRotator ThunderRot = GetActorRotation() + FRotator(0, 90 * DeltaTime / 2, 0);
		if (ThunderFloatingTime / FloatingSpeed <= 1)
		{
			FVector ThunderLoc = UKismetMathLibrary::VLerp(StartLoc, StartLoc + GetActorUpVector() * 20, ThunderFloatingTime / FloatingSpeed);
			ServerTransformSet(ThunderLoc, ThunderRot);
		}
		else if (ThunderFloatingTime / FloatingSpeed <= 2)
		{
			FVector ThunderLoc = UKismetMathLibrary::VLerp(StartLoc + GetActorUpVector() * 20, StartLoc, ThunderFloatingTime / FloatingSpeed - 1);
			ServerTransformSet(ThunderLoc, ThunderRot);
		}
		else
		{
			ThunderFloatingTime = 0;
		}
	}
}

void AThunder::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	class AAOEThunder* AOE = Cast<AAOEThunder>(OtherActor);
	if (AOE)
	{
		ServerDetach();
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		IsDirectionSet = true;
		StartLoc = GetActorLocation();
		SetActorRotation(FRotator(0, 0, 0));
	}
}

void AThunder::ServerTransformSet_Implementation(FVector ThunderLoc, FRotator ThunderRot)
{
	SetActorLocation(ThunderLoc);
	SetActorRotation(ThunderRot);
}

void AThunder::ServerDetach_Implementation()
{
	MulticastDetach();
}

void AThunder::MulticastDetach_Implementation()
{
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
