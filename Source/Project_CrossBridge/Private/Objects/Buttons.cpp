// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Buttons.h"
#include "Components/BoxComponent.h"

// Sets default values
AButtons::AButtons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	ButtonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonComp"));
	ButtonComp->SetupAttachment(RootComponent);
	PushButtonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PushButtonComp"));
	PushButtonComp->SetupAttachment(RootComponent);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AButtons::OnOverlap);
}

// Called when the game starts or when spawned
void AButtons::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtons::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
