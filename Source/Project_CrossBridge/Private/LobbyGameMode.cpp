// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Player의 숫자를 저장한다.
	int32 NumberofPlayers = GameState.Get()->PlayerArray.Num();

	UE_LOG(LogTemp,Warning,TEXT("Player NUm : %d"),NumberofPlayers);
	if(NumberofPlayers == 2)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/Bridge?listen"));
			UE_LOG(LogTemp,Warning,TEXT("Go Lobby"));
		}
	}
	
}
