// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/SwordTarget.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ASwordTarget::ASwordTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(false);
	SetRootComponent(MeshComp);
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(MeshComp);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASwordTarget::OnOverlap);
}

// Called when the game starts or when spawned
void ASwordTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwordTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordTarget::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel13)
	{
		MeshComp->SetSimulatePhysics(true);
		GetWorld()->SpawnActor<AActor>(BreakActor, GetActorLocation(), GetActorRotation());
		FTimerHandle DestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer,
			FTimerDelegate::CreateLambda([this]()->void
				{
					Destroy();
				}), 2, false);
	}
}
