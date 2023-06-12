// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PracticeActorSpawn.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API APracticeActorSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APracticeActorSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY()
		class APortal* SuccessCheckActor;
	UPROPERTY()
		class USkillTestCompleteWidget* SuccessCheck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* BulletTestWidget;
	UPROPERTY()
		int32 BulletPattern=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBulletTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABulletTarget> SpawnBulletTarget1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABulletTarget> SpawnBulletTarget2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABulletTarget> SpawnBulletTarget3;

	UPROPERTY()
		class ABulletTarget* BulletTarget;

	UFUNCTION()
		void SpawnObject();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* BlackholeCheckComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* DummyFallCheckComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* BlackholeTestWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsBlackholeTest;
	UPROPERTY()
		bool IsTestActor1;
	UPROPERTY()
		bool IsTestActor2;
	UPROPERTY()
		FVector TestActorLoc1;
	UPROPERTY()
		FVector TestActorLoc2;
	UFUNCTION()
		void OnBlackholeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnDummyFallOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
		class ABaseCharacter* TestActor1;
	UPROPERTY()
		class ABaseCharacter* TestActor2;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* TrashCheckComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* TrashTestWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsTrashTest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TrashCount;

	UFUNCTION()
		void OnTrashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* SwordTestWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsSwordTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ASwordTarget> SpawnSwordTarget;
	UPROPERTY()
		class ASwordTarget* SwordTarget;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* HealTestComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* HealTestWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHealTest;
	UPROPERTY()
		bool IsHit;
	UPROPERTY()
		bool IsCheck;
	UFUNCTION()
		void OnHealOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class USoundBase* ClearSound;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class USoundBase* CompleteSound;

};
