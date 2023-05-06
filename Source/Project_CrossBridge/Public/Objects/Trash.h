// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trash.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ATrash : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	bool IsActivate;
	bool IsOverlap;

	bool GetActivate();

	UFUNCTION(Server, Unreliable)
		void ServerActivate(FVector SpawnLoc);

	UFUNCTION(Server, Unreliable)
		void ServerDeactivate();

	UFUNCTION(Server, Unreliable)
		void ServerPhysicsSet();

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
