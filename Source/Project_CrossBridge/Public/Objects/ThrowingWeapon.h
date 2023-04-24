// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowingWeapon.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AThrowingWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowingWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponSpeed = 50;
};
