// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// 기본공격
	void BasicAttack(const FVector& HitTarget);

	// 라인트레이스를 이용한 공격
	void LineTraceFire(const FTransform SocketTransform);

private:
	// projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> Projectile;
	
	
};
