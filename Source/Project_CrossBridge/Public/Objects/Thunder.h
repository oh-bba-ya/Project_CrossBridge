// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Thunder.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AThunder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThunder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxComp;
	
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Unreliable)
		void ServerTransformSet(FVector ThunderLoc, FRotator ThunderRot);
	UFUNCTION(Server, Unreliable)
		void ServerDetach();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastDetach();

	UPROPERTY()
		FVector StartLoc;
	
	UPROPERTY()
		float ThunderFloatingTime;
	UPROPERTY()
		float FloatingSpeed = 2;
	UPROPERTY()
		bool IsDirectionSet;




};
