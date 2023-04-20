// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Blackhole.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/BaseCharacter.h"

// Sets default values
ABlackhole::ABlackhole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackhole::OnOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ABlackhole::OnEndOverlap);
}

// Called when the game starts or when spawned
void ABlackhole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Target)
	{
		if (IsBlackholeActive)
		{
			ServerBlackholeActive();
		}
	}

}

void ABlackhole::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Target = Cast<ABaseCharacter>(OtherActor);

}
void ABlackhole::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Target = NULL;
}

void ABlackhole::ServerBlackholeActive_Implementation()
{
	FVector Dir = (GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
	float Dist = GetDistanceTo(Target);
	Target->AddMovementInput(Power * Dir / Dist);
}