// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashCanProjectile.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ATrashCanProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrashCanProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION(Category="Settings|Properties")
	void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UBoxComponent* BoxComponent;

	UFUNCTION()
	void Fire(FVector targetLoc);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float damagePower = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float arcValue = 0.7f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float DestroyTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	bool bDrawLine = false;


	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Sounds")
	class USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Sounds")
	class USoundAttenuation* HitAttenuation;
	

};
