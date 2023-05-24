// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blackhole.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ABlackhole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackhole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* BlackSphereMeshComp;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ShineSphereMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* FloorMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* CenterNiagaraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* ExternalNiagaraComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector OriginSize = FVector(0.2);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LimitSize = FVector(0.4);

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(Replicated)
		class ABaseCharacter* Target1;
	UPROPERTY(Replicated)
		class ABaseCharacter* Target2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Power = 50;
	UPROPERTY()
	bool IsBlackholeActive = false;
	UPROPERTY(Replicated)
		bool IsTarget1;
	UPROPERTY(Replicated)
		bool IsTarget2;
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastBlackholeActive(int32 Num, FVector Input);
	
	void BlackholeActiveSetting();
	void BlackholeDeactivate();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastBlackholeActiveSetting();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastBlackholeDeactivate();


};
