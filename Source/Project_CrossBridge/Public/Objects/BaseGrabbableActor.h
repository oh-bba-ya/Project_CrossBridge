// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGrabbableActor.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ABaseGrabbableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGrabbableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MeshComp;

	UPROPERTY()
		class ABaseCharacter* PCPlayer;

	void FindOwner();

	UFUNCTION()
	void OnGrabbableActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnGrabbableActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);


	UPROPERTY()
	bool IsThrow;
	

};
