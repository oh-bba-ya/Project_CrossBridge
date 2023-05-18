// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletTarget.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ABulletTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxComp;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UGeometryCollectionComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHorizontal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsVertical;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsWhirl;

	UPROPERTY()
		float Rate;
	UPROPERTY()
		int32 SkillRate;

	FVector StartLoc;
	FVector ReturnPoint1;
	FVector ReturnPoint2;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> BreakActor;

	FVector SpawnPoint;
};
