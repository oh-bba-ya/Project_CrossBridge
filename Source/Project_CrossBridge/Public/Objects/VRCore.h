// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRCore.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AVRCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRCore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void SetHealth(float v);

	UFUNCTION()
	void AddHealth(float v);

	UFUNCTION()
	void SubtractHealth(float v);

	UFUNCTION()
	void OnTakeDamage(float v);

	UFUNCTION(Server,Unreliable)
	void Server_OnTakeDamage(float v);

	FORCEINLINE float GetHealth() const {return CureentHealth;}

	UFUNCTION()
	void ChangeColor();
	

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Settings|Properties")
	float CureentHealth;

	UPROPERTY(ReplicatedUsing = ChangeColor)
	FVector linearColor;

	FLinearColor initColor;
	UMaterialInstanceDynamic* dynamicMat;

	

};
