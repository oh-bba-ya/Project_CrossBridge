// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashCanProjectile.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ATrashCanProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrashCanProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UBoxComponent* BoxComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category=Movement)
	class UProjectileMovementComponent* MovementComponent;

};
