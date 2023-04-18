// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VRPlayer.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API AVRPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputMappingContext* IMC_VRInput;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputMappingContext* IMC_VRHand;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_Turn;	
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_LeftIndexCurl;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_LeftGrasp;	
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_LeftY;
		UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_RightIndexCurl;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_RightGrasp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UCameraComponent* VRCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class USkeletalMeshComponent* RightHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UMotionControllerComponent* LeftHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UMotionControllerComponent* RightHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UMotionControllerComponent* LeftGrip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UMotionControllerComponent* RightAim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UMotionControllerComponent* LeftAim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* LeftHandBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* RightHandBox;
	UPROPERTY()
		class UMaterialInstanceDynamic* LeftHandMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* RightHandMat;


	void Move(const FInputActionValue& Values);
	void Turn(const FInputActionValue& Values);
	void LeftIndexCurl();
	void LeftGrasp();
	void LeftY();
	void RightIndexCurl();
	void RightGrasp();
	void	LeftIndexCurlEnd();
	void	LeftGraspEnd();
	void LeftYEnd();
	void	RightIndexCurlEnd();
	void RightGraspEnd();

	UFUNCTION()
	void OnLeftHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRightHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnLeftHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnRightHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FVector RightPrevLoc;
	FQuat RightPrevRot;

	FVector RightThrowDir;
	FQuat RightThrowRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ThrowPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ToquePower;

	UPROPERTY()
		float LeftYTimer;
	
	bool IsLeftIndexCurl;
	bool IsLeftGrasp;
	bool IsLeftY;
	bool IsRightIndexCurl;
	bool IsRightGrasp;
	
	bool IsLeftGrab;
	bool IsRightGrab;

	UPROPERTY()
		class ABaseGrabbableActor* GrabbedActorLeft;
	UPROPERTY()
		class ABaseGrabbableActor* GrabbedActorRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseGrabbableActor> SpawnGrabbedActor;


	void GrabTheActor(ABaseGrabbableActor* GrabbedActor, FString GrabPosition);
	void UnGrabTheActor(ABaseGrabbableActor* GrabbedActor, FString GrabPosition);
	
	void SetGrabInfo();

	void ColorChange(float Rate, FString Position);
	void ResetColorChange(FString Position);

	UFUNCTION(Server, Unreliable)
		void ServerColorChange(float Rate, const FString& Position);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastColorChange(float Rate, const FString& Position);
	UFUNCTION(Server, Unreliable)
		void ServerResetColorChange(const FString& Position);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastResetColorChange(const FString& Position);
	UFUNCTION(Server, Unreliable)
		void ServerSpawnGrabbableActor();


	
};

