// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/SpeedItem.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacter.h"

void ASpeedItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* PCPlayer = Cast<ABaseCharacter>(OtherActor);

	if(PCPlayer != nullptr)
	{
		if(!(PCPlayer->IsVR))
		{
			PCPlayer->SpeedUp();
			//UE_LOG(LogTemp,Warning,TEXT("Speed Up"));
			Destroy();
		}
	}
}

void ASpeedItem::Destroyed()
{
	Super::Destroyed();
	//UE_LOG(LogTemp,Warning,TEXT("Destyroy"));
	if(NiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NiagaraSystem,GetActorLocation());
	}
}
