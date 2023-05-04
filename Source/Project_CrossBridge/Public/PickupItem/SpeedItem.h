// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem/BasePickupItem.h"
#include "SpeedItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ASpeedItem : public ABasePickupItem
{
	GENERATED_BODY()

protected:
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void Destroyed() override;
	
};
