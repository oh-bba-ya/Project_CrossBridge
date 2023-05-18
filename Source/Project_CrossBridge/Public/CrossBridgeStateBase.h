// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CrossBridgeStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ACrossBridgeStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetStart() const {return bIsStart;}
	FORCEINLINE void SetStart(bool s) { bIsStart = s;}

	UFUNCTION(BlueprintCallable)
	bool BpGetStart();

	UFUNCTION(BlueprintCallable)
	void BPSetStart(bool b);

	FORCEINLINE void AddOffsetTime(float deltaTime) {OffsetTime += deltaTime;}
	FORCEINLINE float GetOffsetTime() const {return OffsetTime;}
	
private:
	bool bIsStart = false;

	UPROPERTY(Replicated)
	float OffsetTime = 0.f;
	
	
};
