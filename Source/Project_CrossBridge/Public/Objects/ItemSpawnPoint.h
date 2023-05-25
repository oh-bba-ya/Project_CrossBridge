// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnItem();

protected:
	void SpawnItemTimerFinished();

	// Destroy 델리게이트 파라미터 맞추기.., 델리게이트이므로 UFUNCTION() 매크로 추가..
	UFUNCTION()
	void StartSpawnItemTimer(AActor* DestroyedActor);

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	TArray<TSubclassOf<class ABasePickupItem>> Items;


	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float SpawnDelayTime = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class ABasePickupItem* Item;

	class ACrossBridgeStateBase* BridgeState;


};
