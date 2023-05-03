// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingItem.generated.h"


UCLASS()
class PROJECT_CROSSBRIDGE_API AHomingItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class ABaseCharacter* Ownerplayer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void PickUp(class ABaseCharacter* player);
	
	UFUNCTION(Server, Unreliable)
	void Server_PickUp(class ABaseCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PickUp(class ABaseCharacter* player);
	
	UFUNCTION()
	void DropItem(class ABaseCharacter* player);
	
	UFUNCTION(Server,Unreliable)
	void Server_DropItem(class ABaseCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCast_DropItem(class ABaseCharacter* player);

	UFUNCTION()
	void UsingItem(class ABaseCharacter* player);

	UFUNCTION(Server, Unreliable)
	void Server_UsingItem(class ABaseCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UsingItem(class ABaseCharacter* player);

	

	

};
