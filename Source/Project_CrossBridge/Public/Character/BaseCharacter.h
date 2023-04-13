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
#pragma region Weapon Properties
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

protected:
	void DropWeapon();
	

#pragma endregion 
	
};
