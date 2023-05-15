// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/BasePickupItem.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ABasePickupItem::ABasePickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionProfileName(FName("ItemPreset"));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));

}

// Called when the game starts or when spawned
void ABasePickupItem::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(BindOverlapTimeHandle,this,&ABasePickupItem::BindOverlap,BindOverlapTime,false);
	}
	
}


// Called every frame
void ABasePickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePickupItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* PCPlayer = Cast<ABaseCharacter>(OtherActor);

	if(PCPlayer != nullptr)
	{
		
	}
}

void ABasePickupItem::BindOverlap()
{
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ABasePickupItem::OnBeginOverlap);
}

