// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/HealthItem.h"
#include "NiagaraComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"


void AHealthItem::BeginPlay()
{
	Super::BeginPlay();
}

void AHealthItem::Destroyed()
{
	Super::Destroyed();
	UE_LOG(LogTemp,Warning,TEXT("Destyroy"));
	if(NiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NiagaraSystem,GetActorLocation());
	}
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

