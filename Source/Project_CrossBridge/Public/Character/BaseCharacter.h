// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasePlayer Camera Settings")
		class USpringArmComponent* springArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasePlayer Camera Settings")
		class UCameraComponent* camComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputMappingContext* BaseContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputAction* InputMovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputAction* InputJumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputAction* InputAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
	class UInputAction* InputContextualAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
	class UInputAction* InputDropWeaponAction;
	
	
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 하위 클래스에서 캐릭터마다 슈퍼점프, 일반점프, 제트팩 시스템을 구현할 예정이므로 virtual 키워드를 사용했다.
	virtual void Jump() override;

	virtual void Attack();

	virtual void ContextualActionPressed();

	virtual void ContextualActionReleased();
	


	/** overheadwidget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	
private:
	class UGameInstance* GameInstance;

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	/** JetPack*/
#pragma region Jetpack
protected:
	void Release_Jump();

public:
	FORCEINLINE bool IsFlying() const {return bJetPackActive;}
	FORCEINLINE float GetFuel() const {return Fuel;}
	FORCEINLINE float GetMaxFuel() const {return MaxFuel; }

private:
	FTimerHandle fuelTimer;

	UPROPERTY(Replicated)
	bool bJetPackActive = false;

	UPROPERTY(EditAnywhere, Replicated, Category=JetPackSettings)
	float Fuel = 10;

	UPROPERTY(EditAnywhere,Category=JetPackSettings)
	float MaxFuel = Fuel;

	UPROPERTY(EditAnywhere, Category=JetPackSettings)
	float FuelConsumptionSpeed = 0.03f;

	UPROPERTY(EditAnywhere, Category=JetPackSettings)
	float FuelRechargeSpeed = 0.03f;

	UPROPERTY(EditAnywhere, Category=JetPackSettings)
	float FuelRechargeDelay = 1.f;

	void FillUpFuel();
	
	void FuelConsumption(float value);
	
	UFUNCTION()
	void ActivateJetPack();

	UFUNCTION()
	void DeActivateJetPack();

	UFUNCTION(Server, Unreliable)
	void Server_ActivateJetPack();
	
	UFUNCTION(Server, Unreliable)
	void Server_DeActivateJetPack();



#pragma endregion

	/** Player Status HP, TakeDamage... */
#pragma region Health
public:
	FORCEINLINE float GetMaxHP() const {return MaxHP;}
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }

protected:
	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

private:
	UFUNCTION()
	void PlusHealth(int32 value);

	UFUNCTION()
	void SubTractHealth(int32 value);
	
	UPROPERTY(EditDefaultsOnly, Category="Settings|Status Health")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Status Health")
	float CurrentHP;


#pragma endregion


	/** Fire */
#pragma region Weapon Fire
protected:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Animation")
	UAnimMontage* fireMontage;

	UFUNCTION()
	void Fire();
	
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_Fire();
#pragma endregion

	
	/** Weapon Pickup , Drop */
#pragma region Weapon Properties
private:
	class AProjectileWeapon* myWeapon;
public:
	FORCEINLINE void SetWeapon(AProjectileWeapon* w) { myWeapon = w;}
	FORCEINLINE AProjectileWeapon* GetOwningWeapon() {return myWeapon;}

protected:
	void DropWeapon();

#pragma endregion


/** Player Name*/
#pragma region PlayerName

public:

	UPROPERTY(Replicated)
	FString myName;
	
	UFUNCTION(Server, Unreliable)
	void ServerSetName(const FString& name);
	
	class UOverheadWidget* overhead;

#pragma endregion 
	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	void VRMove(const FInputActionValue& Values);
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
