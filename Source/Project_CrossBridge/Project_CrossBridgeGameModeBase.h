// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project_CrossBridgeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API AProject_CrossBridgeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;


protected:
	void virtual BeginPlay() override;
	
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AVRPlayer> VRUser;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Start Player Nums")
	int32 StartPlayerNums = 3;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings|Time")
	float WarmupTime = 30.f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Time")
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Time")
	float LevelStartingTime = 0.f;
	

};
