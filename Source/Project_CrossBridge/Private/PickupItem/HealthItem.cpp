// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/HealthItem.h"
#include "NiagaraComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AHealthItem::AHealthItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionProfileName(FName("ItemPreset"));

	NiagraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagraComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHealthItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AHealthItem::OnBeginOverlap);
	
}

void AHealthItem::Destroyed()
{
	if(NiagraDestroyComp)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NiagraDestroyComp,GetActorLocation());
	}
}

// Called every frame
void AHealthItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Item Collision"));
	ABaseCharacter* PCPlayer = Cast<ABaseCharacter>(OtherActor);
	if(PCPlayer)
	{
		if(!(PCPlayer->IsVR))
		{
			UE_LOG(LogTemp,Warning,TEXT("PC Collision"));
			PCPlayer->Server_RecoveryHP(HealingValue);
			Destroy();
		}
	}
}

