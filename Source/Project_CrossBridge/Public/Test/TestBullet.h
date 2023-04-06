// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestBullet.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ATestBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category=BulletSettings)
	class USphereComponent* sphereComp;

	UPROPERTY(EditDefaultsOnly, Category=BulletSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category=BulletSettings)
	float moveSpeed = 300;

	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
};
