// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CrossBridgeState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ACrossBridgeState : public AGameState
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetStart() const {return bIsStart;}
	FORCEINLINE void SetStart(bool s) { bIsStart = s;}

	UFUNCTION(BlueprintCallable)
	bool BpGetStart();

	UFUNCTION(BlueprintCallable)
	void BPSetStart(bool b);
private:
	bool bIsStart = false;
	
};
