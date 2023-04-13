// Copyright Epic Games, Inc. All Rights Reserved.


#include "Project_CrossBridgeGameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "VRCharacter/VRPlayer.h"

AProject_CrossBridgeGameModeBase::AProject_CrossBridgeGameModeBase()
{
	//ConstructorHelpers::FClassFinder<AVRPlayer> VRUser(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VRPlayer/BP_VRPlayer.BP_VRPlayer_C'"));
	//if (VRUser.Succeeded())
	//{
	//	DefaultPawnClass = VRUser.Class;
	//}
}

void AProject_CrossBridgeGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
	
		DefaultPawnClass = AVRPlayer::StaticClass();
	}
   
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
