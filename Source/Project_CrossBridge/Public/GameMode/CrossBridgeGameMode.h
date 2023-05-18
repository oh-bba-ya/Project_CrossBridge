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

protected:
	void virtual BeginPlay() override;

public:
	void ResetGame();

	UPROPERTY(EditDefaultsOnly)
	bool bisReset = false;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Time")
	float WarmupTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Time")
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Time")
	float LevelStartingTime = 0.f;


private:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
};
