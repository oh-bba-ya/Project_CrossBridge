// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingProjectile.generated.h"



UCLASS()
class PROJECT_CROSSBRIDGE_API AHomingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MissileMeshComponent;
	

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Sounds")
	class USoundBase* hitSounds;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Sounds")
	class USoundAttenuation* hitAttenuation;

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float Damage = 10.f;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UNiagaraSystem* ExplosionEffect;

};
