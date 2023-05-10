// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/GarbageItem.h"

#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Weapon/TrashCan.h"

AGarbageItem::AGarbageItem()
{
	Count = MaxCount;
	BoxComponent->SetCollisionProfileName(FName("ItemPreset"));
}

void AGarbageItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);
	if(player != nullptr)
	{
		if(!(player->IsVR))
		{
			if(player->GetTrashCan() != nullptr)
			{
				player->GetTrashCan()->SetCount(Count);
				Destroy();
			}
		}
	}
}
