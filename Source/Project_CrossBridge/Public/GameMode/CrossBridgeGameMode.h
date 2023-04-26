// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CrossBridgeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ACrossBridgeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	void ResetGame();

	bool bisReset = false;
	
};
