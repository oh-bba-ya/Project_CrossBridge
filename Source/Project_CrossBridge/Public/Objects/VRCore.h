// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "VRCore.generated.h"

DECLARE_DYNAMIC_DELEGATE(FDynamiceCoreDelegate);

UCLASS()
class PROJECT_CROSSBRIDGE_API AVRCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRCore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UStaticMeshComponent* MeshComponent;


	// VR유저가 이길시.. 레이저발사
	UFUNCTION()
	void LaserFire();

	// 레이저 재생 멈추고 실드 파괴 콜백 실행..
	UFUNCTION()
	void EndLaser();

	// PC유저가 이긴다면 VR코어 추락..
	UFUNCTION()
	void FallingVRCore();

	// 시티 에너지 실드 함수가 바인딩된 델리게이트..
	FDynamiceCoreDelegate coreDelegate;


	/** VR Core Status */
private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Status Health")
	float MaxHP = 100.f;

	UPROPERTY(EditDefaultsOnly,ReplicatedUsing = OnRep_CurrentHealth, Category="Settings|Status Health")
	float CurrentHP;

	
public:
	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	void OnHealthUpdate();

	FORCEINLINE float GetMaxHP() const {return MaxHP;}
	FORCEINLINE float GetHealth() const { return CurrentHP; }

	UFUNCTION(Category="Settings|Status Health")
	void SetCurrentHealth(float value);


	UFUNCTION(BlueprintCallable, Category = "Settings|Status Health")
	void OnTakeDamage(float d);
	

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UNiagaraComponent* laserComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	class UNiagaraComponent* ChargeComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings|Properties")
	bool bIsFire = false;
	

	class ACrossBridgeStateBase* BridgeState;

	UPROPERTY(EditDefaultsOnly,Category="Settings|Falling")
	class ACoreTarget* Target;

	UPROPERTY(EditDefaultsOnly,Category="Settings|Falling")
	float Speed = 3.f;

	UPROPERTY(EditDefaultsOnly,Category="Settings|Falling")
	float Distance = 100.f;

	UPROPERTY(EditDefaultsOnly,Category="Settings|Falling")
	float FallingCallTime = 0.01f;


	UPROPERTY(EditDefaultsOnly,Category="Settings|Falling")
	class UNiagaraComponent* VRCoreDestroyComp;
};


