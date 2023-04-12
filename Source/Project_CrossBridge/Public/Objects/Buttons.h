// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Buttons.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AButtons : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AButtons();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ButtonComp;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* PushButtonComp;
	
	
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
