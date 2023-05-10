// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashCan.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ATrashCan : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrashCan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void PickUpTrashCan(class ABaseCharacter* player);
	
	UFUNCTION(Server, Unreliable)
	void Server_PickUpTrashCan(class ABaseCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PickUpTrashCan(class ABaseCharacter* player);

	FORCEINLINE bool GetbFireDelay() {return bFireDelay;}

	FORCEINLINE float GetCount() const {return Count;}
	FORCEINLINE void SetCount(float v) {Count += v;} 
private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	float Count = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float FireCanCount = 2.f;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	float FireDelayTime = 2.0f;
	

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	class ABaseCharacter* OwnerPlayer;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Properties")
	bool bFireDelay = true;

public:
	UFUNCTION()
	void Fire(class ABaseCharacter* player,const FVector hitTarget);

	UPROPERTY(EditDefaultsOnly, Category="Settings|Projectile")
	TSubclassOf<class ATrashCanProjectile> projectileFactory;

	UFUNCTION(Server, Unreliable)
	void Server_Fire(class ABaseCharacter* player, const FVector hitTarget);

};
