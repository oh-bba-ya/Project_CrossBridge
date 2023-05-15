// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickupItem.h"
#include "BasePickupItem.h"
#include "HealthItem.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AHealthItem : public ABasePickupItem
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float HealingValue = 10.f;

	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;



};
