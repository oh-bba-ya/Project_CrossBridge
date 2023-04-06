// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_CROSSBRIDGE_API UGunCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunCombatComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ACBelica의 Private , Protected에 접근 할 수 있도록 friend 설정.
	friend  class ACBelica;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class ACBelica* Character;
	
/** Aiming */
#pragma region Aiming
	void SetAiming(bool bisAiming);

	UFUNCTION(NetMulticast, Reliable)
	void MulitCastSetAiming(bool bIsAiming);
	
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);
	
	
	UPROPERTY(Replicated)
	bool bAiming;
#pragma endregion


	/* HitTarget */
#pragma region HitTarget
public:
	FORCEINLINE float GetTraceLength() const {return TraceLength;}

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	
private:
	FVector HitTarget;

	UPROPERTY(EditAnywhere)
    float TraceLength = 80000.f;

	
#pragma endregion

	/** Fire */
#pragma region Fire
public:
	void FireButtonPressed(bool bPressed);

	// 기본공격
	void BasicAttack();

	void Fire();

	void Server_Fire();

	void Multicast_Fire();


private:
	bool bFireButtonPressed;
	
	// projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileFactory;
	
	
};
