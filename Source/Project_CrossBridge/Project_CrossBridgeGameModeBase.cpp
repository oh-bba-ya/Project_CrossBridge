// Copyright Epic Games, Inc. All Rights Reserved.


#include "Project_CrossBridgeGameModeBase.h"

#include "BaseCharacterController.h"
#include "CrossBridgeStateBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

#include "HeadMountedDisplayFunctionLibrary.h"



void AProject_CrossBridgeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}


void AProject_CrossBridgeGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}


AActor* AProject_CrossBridgeGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	for(TActorIterator<APlayerStart> iter(GetWorld());iter;++iter)
	{
		APlayerStart* ps = *iter;
		if(ps->PlayerStartTag != FName("Spawned"))
		{
			ps->PlayerStartTag = FName("Spawned");
			return ps;
		}
		
	}

	return nullptr;
}
