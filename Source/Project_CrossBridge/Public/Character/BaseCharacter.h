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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
	class UInputAction* InputRollingAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
	class UInputAction* InputSlidingAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
	class UInputAction* InputQuitAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
	class UInputAction* InputPickupAction;
	
	
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 하위 클래스에서 캐릭터마다 슈퍼점프, 일반점프, 제트팩 시스템을 구현할 예정이므로 virtual 키워드를 사용했다.
	virtual void Jump() override;

	virtual void Attack();

	virtual void ContextualActionPressed();

	virtual void ContextualActionReleased();

/** Sliding, Rolling */
#pragma region Sliding, Rolling Action
private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Animation")
	UAnimMontage* RollingMontage;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Animation")
	UAnimMontage* SlidingMontage;
protected:

	void RollingActionPressed();

	void RollingActionReleased();
	
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_RollingActionPressed();

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_RollingActionReleased();

	void SlidingActionPressed();
	void SlidingActionRelease();

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_SlidingActionPressed();

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_SlidingActionRelease();
	
#pragma endregion 

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
	UPROPERTY(EditDefaultsOnly, Category="Settings|JectPack")
	float JetPackSpeed = 1.f;
	
	FTimerHandle fuelTimer;

	UPROPERTY(Replicated)
	bool bJetPackActive = false;

	UPROPERTY(EditAnywhere, Replicated, Category="Settings|JetPack")
	float Fuel = 10;

	UPROPERTY(EditAnywhere,Category="Settings|JetPack")
	float MaxFuel = Fuel;

	UPROPERTY(EditAnywhere, Category="Settings|JetPack")
	float FuelConsumptionSpeed = 0.03f;

	UPROPERTY(EditAnywhere, Category="Settings|JetPack")
	float FuelRechargeSpeed = 0.03f;

	UPROPERTY(EditAnywhere, Category="Settings|JetPack")
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

	UFUNCTION(BlueprintCallable,Server, Unreliable)
	void Server_TakeDamage(float value);

	UFUNCTION(BlueprintCallable,Server, Unreliable)
	void Server_RecoveryHP(float value);
	

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


	/** Skill : Freeze */
#pragma region Skill Freeze
private:
	UPROPERTY(EditDefaultsOnly,Category="Settings|Skill Freeze")
	TSubclassOf<class AFreeze> FreezeFactory;

	UPROPERTY(Replicated)
	class AFreeze* freeze;
protected:

	void FreezeSpawn();
	
	UFUNCTION(Server,Unreliable)
	void Server_FreezeSpawn();

	void RemoveFreeze();

	UFUNCTION(Server,Unreliable)
	void Server_RemoveFreeze();


#pragma endregion 


	/** Canon Fire */
#pragma region Canon
public:
	UFUNCTION()
	void CanonFire();
	
	class ACannon* mycanon;

	FORCEINLINE void SetHomingItem(class AHomingItem* homing) {HomingItem = homing;}

private:
	UPROPERTY(EditDefaultsOnly, Replicated, Category="Settings|Cannon")
	class AHomingItem* HomingItem;

#pragma endregion 


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		class UInputAction* IA_LeftX;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_RightIndexCurl;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_RightGrasp;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_RightB;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* IA_RightA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UCameraComponent* VRCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class USkeletalMeshComponent* HeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* HeadComp;
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
		class UMotionControllerComponent* RightGrip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UMotionControllerComponent* RightAim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UMotionControllerComponent* LeftAim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* LeftHandBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* RightHandBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UStaticMeshComponent* SwordMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* SwordComp;

	UPROPERTY()
		class UMaterialInstanceDynamic* HeadMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* LeftHandMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* RightHandMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* SwordMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* VRStatusWidget;


	void VRMove(const FInputActionValue& Values);
	void Turn(const FInputActionValue& Values);
	void LeftIndexCurl();
	void LeftGrasp();
	void LeftY();
	void LeftX();
	void RightIndexCurl();
	void RightGrasp();
	void RightB();
	void RightA();
	void	LeftIndexCurlEnd();
	void	LeftGraspEnd();
	void LeftYEnd();
	void LeftXEnd();
	void	RightIndexCurlEnd();
	void RightGraspEnd();
	void RightBEnd();
	void RightAEnd();

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
	UPROPERTY()
	FVector RightThrowDir;
	UPROPERTY()
	FQuat RightThrowRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ThrowPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ToquePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlackHoleForwardPower = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Gravity = -4000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UnitTime = 0.2;

	UPROPERTY()
		float LeftYTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LeftYCastTime = 5;
	UPROPERTY()
		float LeftXTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LeftXCastTime = 5;
	UPROPERTY()
		float BlackholeTimer;
	UPROPERTY()
		float RightBTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RightBCastTime = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VRHealTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VRHealDelayTime = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwordSetTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwordCompleteTime = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwordActivateTime;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwordActivateLimitTime = 5;

	bool IsVR;

	bool IsLeftIndexCurl;
	bool IsLeftGrasp;
	bool IsLeftY;
	bool IsLeftX;
	UPROPERTY(Replicated)
	bool IsBlackholeSet;
	bool IsRightIndexCurl;
	bool IsRightGrasp;
	bool IsRightB;
	bool IsRightA;
	bool IsRedDotSet;
	bool IsSwordSet;

	bool IsLeftGrab;
	bool IsRightGrab;

	UPROPERTY()
		TArray<FVector> LeftXTraces;

	UPROPERTY()
		class ABaseGrabbableActor* GrabbedActorLeft;
	UPROPERTY()
		class ABaseGrabbableActor* GrabbedActorRight;

	UPROPERTY()
		class ABlackhole* Blackhole;
	UPROPERTY()
		class AActor* RedDot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseGrabbableActor> SpawnGrabbedActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABlackhole> SpawnBlackhole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AThrowingWeapon> SpawnThrowingWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> SpawnRedDot;

	UFUNCTION(Server, Unreliable)
		void ServerGrabTheActor(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastGrabTheActor(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition);
	UFUNCTION(Server, Unreliable)
		void ServerUnGrabTheActor(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition, FVector RightDirThrow, FQuat RightRotThrow);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastUnGrabTheActor(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition, FVector RightDirThrow, FQuat RightRotThrow);

	void SetGrabInfo();
	void SetRedDot();

	FVector BlackHoleTrace();

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

	UFUNCTION(Server, Unreliable)
		void ServerVRSetting();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastVRSetting();

	UFUNCTION(Server, Unreliable)
		void ServerPCSetting();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPCSetting();

	UFUNCTION(Server, Unreliable)
		void ServerBlackholeSet(float Rate, FVector Loc);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastBlackholeSet(float Rate, FVector Loc);
	UFUNCTION(Server, Unreliable)
		void ServerBlackholeReset();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastBlackholeReset();

		UFUNCTION(Server, Unreliable)
		void ServerVRTransform(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastVRTransform(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform);
	UFUNCTION(Server, Unreliable)
		void ServerBlackholeActivate(bool IsActivate);

	UFUNCTION(Server, Unreliable)
		void ServerSpawnThrowingWeapon(FVector SpawnLoc, FRotator SpawnRot);

public:
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		float VRCurHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VRMaxHP = 100;

	UFUNCTION(BlueprintCallable)
		void VRGetDamage(float Damage);

	UFUNCTION(Server, Unreliable)
		void ServerVRGetDamage(float Damage);

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastVRGetDamage(float Rate);
	
};
