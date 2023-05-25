// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CityEnergyShield.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ACityEnergyShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACityEnergyShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DissolveShield();

	
private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;
	

	UPROPERTY(EditDefaultsOnly, Category="Settings|VRCore")
	class AVRCore* core;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings|VRCore")
	UMaterialInstanceDynamic* ShieldMat;

	UPROPERTY(EditDefaultsOnly, Category="Settings|VRCore")
	float Value = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings|VRCore")
	float CurDissolve = 0.f;

	// Dissolve 값 증가량..
	UPROPERTY(EditDefaultsOnly, Category="Settings|VRCore")
	float AddDissolve = 0.05f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|VRCore")
	float DissolveHadnleTime = 0.05f;

};
