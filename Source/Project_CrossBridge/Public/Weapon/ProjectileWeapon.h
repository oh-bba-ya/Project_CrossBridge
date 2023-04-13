// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileWeapon.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AProjectileWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|WeaponProperties")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|WeaponProperties")
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Settings|WeaponProperties")
	int32 Ammo;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|WeaponProperties")
	float fireDelay;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|WeaponProperties")
	float Damage;


public:
	UFUNCTION()
	void OnBoxComponentBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Unreliable)
	void Server_PickupWeapon(class ABaseCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PickupWeapon(class ABaseCharacter* player);

public:
	UFUNCTION(Server,Unreliable)
	void Server_DropWeapon(class ABaseCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCast_DropWeapon(class ABaseCharacter* player);

public:
	UFUNCTION()
	void Fire(class ABaseCharacter* player);

	UPROPERTY(EditDefaultsOnly, Category="Settings|Projectile")
	TSubclassOf<class AProjectile> projectileFactory;

private:
	UFUNCTION(Server, Unreliable)
	void Server_Fire(class ABaseCharacter* player);
	
	

};
