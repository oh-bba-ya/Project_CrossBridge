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


private:
	UPROPERTY(EditAnywhere, Category=BelicaSettings)
	TSubclassOf<class UCBelicaWidget> BelicaWidget;

	class UCBelicaWidget* BelicaUI;

	
private:
	/** JetPack */
	FTimerHandle fuelTimer;
	
	UPROPERTY(Replicated)
	bool bJetPackActive = false;

	UPROPERTY(EditAnywhere,Category=JetPackSettings)
	float Fuel = 10;

	UPROPERTY(EditAnywhere,Category=JetPackSettings)
	float MaxFuel = Fuel;

	UPROPERTY(EditAnywhere, Category=JetPackSettings)
	float FuelConsumption = 0.03f;

	UPROPERTY(EditAnywhere, Category=JetPackSettings)
	float FuelRechargeSpeed = 0.03f;

	UPROPERTY(EditAnywhere, Category=JetPackSettings)
	float FuelRechargeDelay = 1.f;


	void FillUpFuel();

	UFUNCTION(Server, reliable)
	void ServerFillUpFuel();

	UFUNCTION()
	void ActivateJetPack();

	UFUNCTION()
	void DeActivateJetPack();

	UFUNCTION(Server, reliable)
	void ServerActivateJetPack();

	
	UFUNCTION(Server, reliable)
	void ServerDeActivateJetPack();

/** TEST Code */
public:
	UPROPERTY(EditDefaultsOnly, Category= MySettings)
	TSubclassOf<class ATestBullet> bulletFactory;

	void Attack() override;
	
	UFUNCTION(Server, Unreliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastAttack();
	
};
