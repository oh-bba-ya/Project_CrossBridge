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
	class UInputAction* InputInterAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
	class UInputAction* InputTrashCanFireAction;
	
	
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 하위 클래스에서 캐릭터마다 슈퍼점프, 일반점프, 제트팩 시스템을 구현할 예정이므로 virtual 키워드를 사용했다.
	virtual void Jump() override;

	virtual void Attack();

	virtual void ContextualActionPressed();

	virtual void ContextualActionReleased();

/** PC Overhead Component */
#pragma region Overhead Component
public:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Overhead")
	class UStaticMeshComponent* overHeadComp;

	UPROPERTY(ReplicatedUsing=OnRep_SetOverheadCompColor)
	FVector linearColor;
	
	FLinearColor initColor;
	UMaterialInstanceDynamic* dynamicMat;

	UFUNCTION()
	void OnRep_SetOverheadCompColor();
#pragma endregion 
	

/** Sliding, Rolling */
#pragma region Sliding, Rolling Action
private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Animation")
	UAnimMontage* RollingMontage;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Animation")
	UAnimMontage* SlidingMontage;
public:

	void RollingActionPressed();

	void RollingActionReleased();

protected:
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_RollingActionPressed();

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_RollingActionReleased();

	UFUNCTION(Server, Unreliable)
	void Server_RollingActionPressed();
	
	UFUNCTION(Server, Unreliable)
	void Server_RollingActionReleased();
	
public:
	void SlidingActionPressed();
	
	void SlidingActionRelease();

protected:
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_SlidingActionPressed();

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_SlidingActionReleased();

	UFUNCTION(Server, Unreliable)
	void Server_SlidingActionPressed();

	UFUNCTION(Server, Unreliable)
	void Server_SlidingActionReleased();
	
#pragma endregion 



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

	UFUNCTION(Category="Settings|Status Health")
	void SetCurrentHealth(float value);


	UFUNCTION(BlueprintCallable, Category = "Settings|Status Health")
	void OnTakeDamage(float d);
	
	UFUNCTION(BlueprintCallable,Server, Unreliable)
	void Server_RecoveryHP(float value);
	

protected:
	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	void OnHealthUpdate();


private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Status Health")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly,ReplicatedUsing = OnRep_CurrentHealth, Category="Settings|Status Health")
	float CurrentHP;


#pragma endregion


/** Player Dead, Hit , Respawn*/
#pragma region PCPlayer Dead, Respawn
public:
	UFUNCTION()
	void PCPlayerDead();
	
	UFUNCTION(Server, Reliable)
	void Server_PCPlayerDead();
	
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_PCPlayerDead();

	FORCEINLINE bool GetPCPlayerDead() const {return bIsDead;}

	
	UFUNCTION()
	void ChangeSpectator();

protected:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Hit();
	
private:
	UPROPERTY(Replicated)
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Animation")
	UAnimMontage* HitMontage;

	/** Fire */
#pragma region Weapon, TrashCan Fire
protected:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Animation")
	UAnimMontage* fireMontage;

	UFUNCTION()
	void Fire();

	UFUNCTION(Server, Unreliable)
	void Server_Fire();
	
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_Fire();

	UFUNCTION()
	void TrashCanFire();
#pragma endregion

	
	/** Equip  Weapon, Item Pickup , Drop */
#pragma region Weapon Properties
private:
	UPROPERTY(VisibleAnywhere,Replicated,Category="Settings|Equip")
	bool bisEquip = false;
	
	UPROPERTY(VisibleAnywhere,Replicated,Category="Settings|Weapon")
	class AProjectileWeapon* myWeapon;

	UPROPERTY(VisibleAnywhere,Replicated,Category="Settings|Item")
	class AHomingItem* myHoming;

	UPROPERTY(VisibleAnywhere,Replicated,Category="Settings|Weapon")
	class ATrashCan* myTrashCan;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Weapon")
	TSubclassOf<class ATrashCan> trashcanFactory;
	
public:
	FORCEINLINE void SetEquip(bool b) { bisEquip = b;}
	FORCEINLINE bool GetIsEquip() {return bisEquip;}
	
	FORCEINLINE void SetWeapon(AProjectileWeapon* w) { myWeapon = w;}
	FORCEINLINE AProjectileWeapon* GetOwningWeapon() {return myWeapon;}

	FORCEINLINE void SetHomingItem(AHomingItem* h) {myHoming = h;}
	FORCEINLINE AHomingItem* GetHomingItem() {return myHoming;}

	FORCEINLINE void SetTrashCan(ATrashCan* t) {myTrashCan = t;}
	FORCEINLINE ATrashCan* GetTrashCan() {return myTrashCan;}

	UFUNCTION()
	void SpawnTrashCan();


	


protected:
	void DropWeapon();

#pragma endregion

	

	/** Skill : Freeze */
#pragma region Skill Freeze
private:

	UPROPERTY(EditDefaultsOnly,Replicated,Category="Settings|Skill Freeze")
	float FreezeCoolTime = 2.0f;

	UPROPERTY(EditDefaultsOnly,Replicated,Category="Settings|Skill Freeze")
	bool bCanFreeze = true;
	
	
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
public:
	FORCEINLINE AFreeze* GetFreeze() {return freeze;}

	FORCEINLINE bool GetCanFreeze() const {return bCanFreeze;}
	FORCEINLINE void SetCanFreeze(bool can) {bCanFreeze = can;}


#pragma endregion 


	/** Canon Fire */
#pragma region Canon
public:
	UFUNCTION()
	void CanonFire();
	
	class ACannon* mycanon;
#pragma endregion 


	/** Speed Up and Down*/
#pragma region SpeedAccelerator
public:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Speed")
	class UNiagaraComponent* DashEffectComponent;
	
	UFUNCTION()
	void SpeedUp();

	FORCEINLINE bool GetIsSpeedUp() {return bIsSpeedUp;}

private:
	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Speed")
	float ReturnSpeedTime = 2.0f;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Speed")
	float DuringSpeedTime = 2.0f;

	UPROPERTY()
	FTimerHandle SpeedHandle;

	float SpeedCurrentTime = 0.f;

	UPROPERTY()
	bool bIsSpeedUp = false;

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SpeedUP();
	
	UFUNCTION()
	void ComeBackSpeed();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_CombackSpeed();



#pragma endregion


	/** Trace CrossHair */
#pragma region Trace Crosshair
public:
	void SetHUDCrosshairs(float DeletaTime);
	
protected:
	void TraceUnderCosshairs(FHitResult& TraceHitResult);
	
private:
	class AWeaponHUD* HUD;

	class ABaseCharacterController* PCController;
	
	FHitResult HitResult;

	FVector HitTarget;
	
	UPROPERTY(EditDefaultsOnly, Category= "Settings|Crosshair")
	float TraceLength = 1000.f;

	bool bDisplayCrosshair = false;
	
	/** Textures for the Weapon crosshairs */
	UPROPERTY(EditAnywhere, Category= "Settings|Crosshair")
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category= "Settings|Crosshair")
	class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category= "Settings|Crosshair")
	class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category= "Settings|Crosshair")
	class UTexture2D* CrosshairsTop;


	UPROPERTY(EditAnywhere, Category= "Settings|Crosshair")
	class UTexture2D* CrosshairsBottom;


#pragma endregion

	/** Converter */
#pragma region Converter
private:
	UPROPERTY(VisibleAnywhere,Replicated,Category="Settings|Weapon")
	class AMaterialConverter* myConverter;

public:
	FORCEINLINE void SetConverter(AMaterialConverter* w) { myConverter = w;}
	FORCEINLINE AMaterialConverter* GetConverter() {return myConverter;}

protected:
	UFUNCTION()
	void UsingConverter();

#pragma endregion


	/** GameOver Widget */
#pragma region PC Win, lose GameOver Widget
protected:
	UFUNCTION()
	void ShowGameOverWidget();

	UFUNCTION(Client ,Reliable)
	void Clinet_ShowGameOverWidget();

private:

	class ACrossBridgeStateBase* BridgeState;
	
	UPROPERTY(EditAnywhere, Category="Settings|HUD")
	TSubclassOf<class UUserWidget> GameOverWidget;

	UPROPERTY()
	class UGameOver* GameOverMenu;

	bool bGameOverMenuOpen = false;


#pragma endregion



	/** HP , JetPack */
private:

	
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
		class  UStaticMeshComponent* VRHeadMesh;
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
		class UStaticMeshComponent* InvisibleSwordMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* SwordComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UNiagaraComponent* GrabbableObjectCreateEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UNiagaraComponent* BlackholeTraceComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		class UNiagaraComponent* BulletAimTraceComp;
		

	UPROPERTY()
		class UMaterialInstanceDynamic* HeadMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* LeftHandMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* RightHandMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* SwordMat;
	UPROPERTY()
		class UMaterialInstanceDynamic* InvisibleSwordMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* VRStatusWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* LeftWidgetInteraction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* RightWidgetInteraction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetInteractionComponent* LeftWidgetInteractionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetInteractionComponent* RightWidgetInteractionComp;


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

	UFUNCTION()
		void OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION()
		void OnWidgetLeftOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnWidgetLeftEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnWidgetRightOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnWidgetRightEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	FVector RightPrevLoc;
	FQuat RightPrevRot;

	UPROPERTY()
	FVector RightThrowDir;
	UPROPERTY()
	FQuat RightThrowRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ThrowPower = 500000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ToquePower = 50.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlackHoleForwardPower = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Gravity = -2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UnitTime = 0.2;

	UPROPERTY()
		float LeftYTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LeftYCastTime = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TrashSpawningPoolTime;
	UPROPERTY()
		float TrashSpawningPoolTimeLimit = 15;
	UPROPERTY()
		float LeftXTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LeftXCastTime = 5;
	UPROPERTY()
		float BlackholeTimer;
	UPROPERTY()
		float BlackholeCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlackholeCoolTimeLimit = 10;
	UPROPERTY()
		float TrashSpawningPoolCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TrashSpawningPoolCoolTimeLimit = 10;
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
		float SwordActivateLimitTime = 10;
	UPROPERTY()
		float SwordCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwordCoolTimeLimit = 15;
	UPROPERTY()
		float SwordDamageCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SwordDamageCoolTimeLimit = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VRReviveLimitTime = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsVR = false;
	
	bool IsVRDead;

	bool IsLeftIndexCurl;
	bool IsLeftGrasp;
	bool IsLeftY;
	bool IsLeftX;
	UPROPERTY(Replicated)
	bool IsBlackholeSet;
	bool IsTrashSpawningPoolCast;
	bool IsTrashSpawningPoolSet;
	bool IsRightIndexCurl;
	bool IsRightGrasp;
	bool IsRightB;
	bool IsRightA;
	bool IsRedDotSet;
	bool IsSwordSet;
	bool IsHeal;

	bool IsLeftGrab;
	bool IsRightGrab;

	bool IsBlackholeCool;
	bool IsSwordCool;
	bool IsSwordDamageCool;
	bool IsTrashSpawningPoolCool;
	bool IsGrabbableActorSpawnCool;
	UPROPERTY()
		TArray<FVector> LeftXTraces;
	UPROPERTY()
		class UVRStatusWidget* VRStatus;
	UPROPERTY()
		class ABaseGrabbableActor* GrabbedActorLeft;
	UPROPERTY()
		class ABaseGrabbableActor* GrabbedActorRight;

	UPROPERTY(Replicated)
		class ABlackhole* Blackhole;
	UPROPERTY(Replicated)
		class ATrashSpawningPool* TrashSpawningPool;
	UPROPERTY()
		class AActor* RedDot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseGrabbableActor> SpawnGrabbedActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AThunder> SpawnThunder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABlackhole> SpawnBlackhole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AThrowingWeapon> SpawnThrowingWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> SpawnRedDot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ATrashSpawningPool> SpawnTrashSpawningPool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> BreakDoor;
	UPROPERTY()
		class APlayerController* VRController;
	UPROPERTY()
		class ABaseCharacterController* VRTimerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHapticFeedbackEffect_Base* BulletCastHaptic;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHapticFeedbackEffect_Base* BulletFireHaptic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHapticFeedbackEffect_Base* SwordCastHaptic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHapticFeedbackEffect_Base* ClickedHaptic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHapticFeedbackEffect_Base* HealHaptic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHapticFeedbackEffect_Base* TrashCastHaptic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHapticFeedbackEffect_Base* BlackholeCastHaptic;





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
		void ServerSpawnGrabbableActor(AActor* OtherActor);

	UFUNCTION(Server, Unreliable)
		void ServerVRSetting();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastVRSetting();

	UFUNCTION(Server, Unreliable)
		void ServerBlackholeSet(float Rate, FVector Loc);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastBlackholeSet(float Rate, FVector Loc);
	UFUNCTION(Server, Unreliable)
		void ServerBlackholeReset();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastBlackholeReset();
	UFUNCTION(Server, Unreliable)
		void ServerTrashSpawningPoolSet(FVector Loc);
	UFUNCTION(Server, Unreliable)
		void ServerTrashSpawningPoolReset();
	UFUNCTION(Server, Unreliable)
		void ServerLeftHandActivate(bool IsActivate);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastLeftHandActivate(bool IsActivate);

		UFUNCTION(Server, Unreliable)
		void ServerVRTransform(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastVRTransform(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform);
	UFUNCTION(Server, Unreliable)
		void ServerBlackholeActivate(bool IsActivate);
	UFUNCTION(Server, Unreliable)
		void ServerTrashSpawningPoolActivate();

	UFUNCTION(Server, Unreliable)
		void ServerSpawnThrowingWeapon(FVector SpawnLoc, FRotator SpawnRot);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VRMaxHP = 100;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		float VRCurHP = VRMaxHP;

	UFUNCTION(BlueprintCallable)
		void VRGetDamage(float Damage);

	UFUNCTION(Server, Unreliable)
		void ServerVRGetDamage(float Damage);

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastVRGetDamage(float Rate);
	UFUNCTION(Server, Unreliable)
		void ServerVRAttack(const FString& Position, class ABaseCharacter* Enemy);


	UFUNCTION()
		void VRRevive();
	UFUNCTION()
		bool VRSkillCheck(FString Position);
	UFUNCTION(Server, Unreliable)
		void ServerVRDoorBreak(const FString& Position,  UPrimitiveComponent* OtherComp);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastVRDoorBreak();

	UPROPERTY()
		class AActor* BreakableDoor;

	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDummy;
};
