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
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AVRPlayer> VRUser;

};
