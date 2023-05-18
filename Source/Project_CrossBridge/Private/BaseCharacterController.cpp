// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterController.h"

#include "CrossBridgeState.h"
#include "CrossBridgeStateBase.h"
#include "CrossPlayerState.h"
#include "Components/TextBlock.h"
#include "GameMode/CrossBridgeGameMode.h"
#include "HUD/BaseCharacterWidget.h"
#include "HUD/ReturnToMainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Project_CrossBridge/Project_CrossBridgeGameModeBase.h"

void ABaseCharacterController::BeginPlay()
{
	Super::BeginPlay();

	Server_CheckMatchState();

	if(baseCharacterWidget != nullptr && IsLocalController())
	{
		baseCharacterUI = CreateWidget<UBaseCharacterWidget>(this,baseCharacterWidget);

		if(baseCharacterUI)
		{
			baseCharacterUI->AddToViewport();
		}
	}

	BridgeState = Cast<ACrossBridgeStateBase>(GetWorld()->GetGameState());
	
}

void ABaseCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(InputComponent == nullptr) return;

	InputComponent->BindAction("Quit", IE_Pressed, this, &ABaseCharacterController::ShowReturnToMainMenu);
	
	
}

void ABaseCharacterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetHUDTime();
	CheckTimeSync(DeltaSeconds);

	

	if(BridgeState != nullptr)
	{
		if(!BridgeState->GetStart())
		{
			BridgeState->AddOffsetTime(DeltaSeconds);
		}
	}
	
	
}

/** Match Time */
#pragma region MatchTime
void ABaseCharacterController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if(IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

float ABaseCharacterController::GetServerTimer()
{
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ABaseCharacterController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if(IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		
	}
}

void ABaseCharacterController::Client_JoinMidgame_Implementation(float Warmup, float Match, float LevelStarting)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	LevelStartingTime = LevelStarting;
}

void ABaseCharacterController::Server_CheckMatchState_Implementation()
{
	BridgeGM = Cast<AProject_CrossBridgeGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(BridgeGM != nullptr)
	{
		WarmupTime = BridgeGM->WarmupTime;
		MatchTime = BridgeGM->MatchTime;
		LevelStartingTime = BridgeGM->LevelStartingTime;
		Client_JoinMidgame(WarmupTime,MatchTime,LevelStartingTime);
	}
}

void ABaseCharacterController::SetHUDCountDown(float CountdownTime)
{
	int32 Min = FMath::FloorToInt(CountdownTime/60.f);
	int32 Seconds = CountdownTime - Min*60;

	FString CountdownText = FString::Printf(TEXT("%02d:%02d"),Min,Seconds);
	if(baseCharacterUI != nullptr)
	{
		baseCharacterUI->CountdownText->SetText(FText::FromString(CountdownText));
	}
	
}

void ABaseCharacterController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime-GetServerTimer());
	if(CountdownInt != SecondsLeft)
	{
		if(BridgeState != nullptr)
		{
			if(BridgeState->GetStart())
			{
				SetHUDCountDown(MatchTime-GetServerTimer() + BridgeState->GetOffsetTime());
				//SetHUDCountDown(MatchTime-GetServerTimer());
			}
			else
			{
				SetHUDCountDown(0);
			}
		}
	}

	CountdownInt = SecondsLeft;
}

void ABaseCharacterController::ServerRequestServerTime_Implementation(float TimeOfClinetRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClinetRequest, ServerTimeOfReceipt);
}

void ABaseCharacterController::ClientReportServerTime_Implementation(float TimeOfClientRequest,
	float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

#pragma endregion 


void ABaseCharacterController::ShowReturnToMainMenu()
{
	if(ReturnToMainMenuWidget == nullptr) return;

	if(ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}

	if(ReturnToMainMenu)
	{
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if(bReturnToMainMenuOpen)
		{
			ReturnToMainMenu->MenuSetup();
		}
		else
		{
			ReturnToMainMenu->MenuTearDown();
		}
	}
	
}

