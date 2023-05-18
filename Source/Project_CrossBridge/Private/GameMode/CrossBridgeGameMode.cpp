// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CrossBridgeGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"


void ACrossBridgeGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ACrossBridgeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bisReset)
	{
		RestartGame();
	}
}


void ACrossBridgeGameMode::ResetGame()
{
	RestartGame();
}




AActor* ACrossBridgeGameMode::ChoosePlayerStart_Implementation(AController* Player)
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

