// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Freeze.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AFreeze : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFreeze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly,Category="Settings|FreezeProperties")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly,Category="Settings|FreezeProperties")
	class UStaticMeshComponent* MeshComponent;

	class ABaseCharacter* player;

	UPROPERTY(EditDefaultsOnly, Category="Settings|FreezeProperties")
	class UNiagaraSystem* breakEffect;
	

};
