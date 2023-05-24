// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CrossBridgeStateBase.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8 //0 ~ 255 
{ 
	Wait,
	Start,
	End,
	Quit
};

UENUM(BlueprintType)
enum class EWinner : uint8 //0 ~ 255 
{
	NONE,
	VR,
	PC
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStateBaseDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSpawnDelegate,class AActor*, DestroyedActor);
/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ACrossBridgeStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACrossBridgeStateBase();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	FORCEINLINE void AddOffsetTime(float deltaTime) {OffsetTime += deltaTime;}
	FORCEINLINE float GetOffsetTime() const {return OffsetTime;}

	
	UFUNCTION(BlueprintCallable)
	void GameMatchState(EGameState e);
	
	FORCEINLINE EGameState GetGameState() const {return state;}
	FORCEINLINE void SetGameState(EGameState e) {state = e;}

	FORCEINLINE EWinner GetWinner() const {return eWinner;}
	FORCEINLINE void SetWinner(EWinner e) {eWinner = e;}

	UFUNCTION(BlueprintCallable)
	void GameWinner(EWinner e);


	FStateBaseDelegate waitStateDelegate;
	FItemSpawnDelegate startStateDelegate;
	FStateBaseDelegate endStateDelegate;
	FStateBaseDelegate quitStateDelegate;

private:

	UFUNCTION()
	void WaitStateCallBack();

	UFUNCTION()
	void StartStateCallBack();

	UFUNCTION()
	void EndStateCallBack();

	UFUNCTION()
	void QuitStateCallBack();

	UPROPERTY()
	bool bStateCall = false;
	
	float OffsetTime = -1.f;

	EGameState state;

	EWinner eWinner;
	
	
};
