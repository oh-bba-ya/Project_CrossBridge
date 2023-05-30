// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComponent; 

	UPROPERTY(VisibleAnywhere, Category=Movement)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float Damage =1.0f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	bool Ready = false;
	
	float currentTime = 0;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float GrowTime = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float ScaleLimit = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float CurrentScale = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float LifeSpan = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UNiagaraSystem* DestroyEffect;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class USoundAttenuation* HitAttenu;
public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void ReadyShoot(float deltaTime);

	
};
