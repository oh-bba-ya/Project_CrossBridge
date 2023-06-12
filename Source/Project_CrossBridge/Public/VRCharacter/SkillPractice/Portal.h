// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* StartPortalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* StartPortalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UNiagaraComponent* StartPortalEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* EndPortalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* EndPortalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* SuccessWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* EndPortalEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* TestEndWidget;

	UFUNCTION()
		void OnStartPortalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndPortalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndPortalVisible();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* PortalSound;

};
