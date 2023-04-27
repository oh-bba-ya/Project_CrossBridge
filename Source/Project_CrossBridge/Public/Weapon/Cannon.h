// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UArrowComponent* Arrow;

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Projectile")
	TSubclassOf<class AHomingProjectile> HomingFactory;


	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Projectile")
	bool bIsFire = false;
	
	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Projectile")
	class AVRCore* core;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Projectile")
	class ABaseCharacter* player;
	
public:
	void HomingFire();

	UFUNCTION(Server,Unreliable)
	void Server_HomingFire();

	UFUNCTION(NetMulticast,Unreliable)
	void MultiCast_HomingFire();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
