// Copyright Epic Games, Inc. All Rights Reserved.


#include "Project_CrossBridgeGameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

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
