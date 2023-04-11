// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "CBelica.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ACBelica : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ACBelica();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Jump() override;

	void Release_Jump();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual  void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UGunCombatComponent* Combat;
	
	
	/** Belica Status (HP..)*/
#pragma region Status
protected:
	UPROPERTY(EditDefaultsOnly, Category="Status Health")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly,Replicated, Category="Status Health")
	float CurrentHP;
	

public:
	FORCEINLINE float GetMaxHP() const {return MaxHP;}
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	
	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	UFUNCTION()
	void PlusHealth(int32 value);

	UFUNCTION()
	void SubTractHealth(int32 value);

	UFUNCTION(Server, Unreliable)
	void Server_TakeDamage(float value);


#pragma endregion 

	
	/** JetPack */
#pragma region JetPack
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

public:
	FORCEINLINE bool IsFlying() const {return bJetPackActive;}
	FORCEINLINE float GetFuel() const {return Fuel;}
	FORCEINLINE float GetMaxFuel() const {return MaxFuel; }

#pragma endregion 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Fire)
	UAnimMontage* FireMontage;

	/**  Aiming */
#pragma region Aiming
protected:
	
	void ContextualActionPressed() override;
	void ContextualActionReleased() override;

	void AimStart();
	void AimEnd();

public:
	bool IsAiming();
#pragma endregion



/** Weapon Properties*/
#pragma region Weapon properties , pickup
private:
	UPROPERTY(ReplicatedUsing= OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

public:
	void SetOverlappingWeapon(AWeapon* Weapon);

#pragma endregion 
	

	
/** TEST Code */
public:
	UPROPERTY(EditDefaultsOnly, Category= MySettings)
	TSubclassOf<class ATestBullet> bulletFactory;

	void Attack() override;
	
	UFUNCTION(Server, Unreliable)
	void Server_Attack();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Attack();

	
	
	
};
