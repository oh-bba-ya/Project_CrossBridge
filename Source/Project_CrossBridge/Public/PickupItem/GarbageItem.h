// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem/BasePickupItem.h"
#include "GarbageItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API AGarbageItem : public ABasePickupItem
{
	GENERATED_BODY()
public:
	AGarbageItem();
private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float Count;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float MaxCount = 1.f;
	
protected:
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
