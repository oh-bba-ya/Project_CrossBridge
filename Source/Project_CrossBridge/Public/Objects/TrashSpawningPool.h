// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashSpawningPool.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ATrashSpawningPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrashSpawningPool();

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
		TSubclassOf<class ATrash> SpawnTrash;

	UPROPERTY()
		TArray<ATrash*> TrashArr;

	void TrashSkillActivate();

	UFUNCTION(Server, Unreliable)
		void ServerSpawnTrash();

	UPROPERTY()
		int32 ArrSize = 10;
	UPROPERTY()
		int32 ArrIndex = 0;

	UPROPERTY()
		float SpawningTimer;
	UPROPERTY()
		float SpawningDelayTime = 2;

	UPROPERTY()
		bool IsActivate;


};
