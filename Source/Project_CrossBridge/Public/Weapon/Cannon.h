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


	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	bool bIsFire = false;
	
	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	class AVRCore* core;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	int32 HommingAmmo = 0;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	bool bFireDelay = true;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	float fireDelayTime = 2.0f;
	
public:
	FORCEINLINE bool GetFireDelay() {return bFireDelay;}
	
	UFUNCTION()
	void HomingFire(class ABaseCharacter* p);

	UFUNCTION(Server,Unreliable)
	void Server_HomingFire(class ABaseCharacter* p);

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_HomingFire(class ABaseCharacter* p, class AHomingProjectile* h);

	UFUNCTION()
	void Entrance(class ABaseCharacter* p);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCast_Entrance(class ABaseCharacter* p);

	UFUNCTION(Server, Unreliable)
	void Server_Entrance(class ABaseCharacter* p);


	UFUNCTION()
	void Exit(class ABaseCharacter* p);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCast_Exit(class ABaseCharacter* p);

	UFUNCTION(Server, Unreliable)
	void Server_Exit(class ABaseCharacter* p);


	UFUNCTION()
	void SetHommingAmmo(int32 v);
	
	UFUNCTION()
	void AddHomingAmmo(int32 v);

	UFUNCTION()
	void SubtractHominAmmo(int32 v);
	
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


/** Homing Item Reload */
public:
	UFUNCTION()
	void Reload(class AHomingItem* homing);

	UFUNCTION(Server, Unreliable)
	void Server_Reload(class AHomingItem* homing);


};

