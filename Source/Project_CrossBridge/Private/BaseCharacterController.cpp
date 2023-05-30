// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterController.h"

#include "CrossBridgeState.h"
#include "CrossBridgeStateBase.h"
#include "CrossPlayerState.h"
#include "Character/BaseCharacter.h"
#include "Components/TextBlock.h"
#include "GameMode/CrossBridgeGameMode.h"
#include "HUD/BaseCharacterWidget.h"
#include "HUD/GameOver.h"
#include "HUD/ReturnToMainMenu.h"
#include "HUD/WeaponHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Project_CrossBridge/Project_CrossBridgeGameModeBase.h"

void ABaseCharacterController::BeginPlay()
{
	Super::BeginPlay();

	Server_CheckMatchState();

	BridgeState = Cast<ACrossBridgeStateBase>(GetWorld()->GetGameState());

	FTimerHandle MatchStateTimer;
	GetWorld()->GetTimerManager().SetTimer(MatchStateTimer,
		FTimerDelegate::CreateLambda([this]()->void
			{
				IsMatchStateSet = true;
			}), 0.1, false);

	
	PCHUD = Cast<AWeaponHUD>(GetHUD());
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

	//SetHUDTime();
	
	if (IsMatchStateSet)
	{
		SetHUDTime();
	}
	
	CheckTimeSync(DeltaSeconds);
	
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

	VRTimer = CountdownText;
	
	if(PCHUD != nullptr)
	{
		PCHUD->CharacterOverlay->CountdownText->SetText(FText::FromString(CountdownText));
	}
	
}

void ABaseCharacterController::SetHUDTime()
{
	float gameTime = 0.f;
	
	if(BridgeState != nullptr)
	{
		if(BridgeState->GetGameState() == EGameState::Wait)
		{
			uint32 SecondsLeft = FMath::CeilToInt(WarmupTime - GetServerTimer());
			if (CountdownInt != SecondsLeft)
			{
				gameTime = WarmupTime - GetServerTimer();
				SetHUDCountDown(gameTime);

				// 만약 대기시간이 종료되었다면.. 게임 시작..
				if(gameTime < 0)
				{
					BridgeState->SetGameState(EGameState::Start);
				}
			}

			CountdownInt = SecondsLeft;
		}
		else if(BridgeState->GetGameState() == EGameState::Start)
		{
			uint32 SecondsLeft = FMath::CeilToInt(MatchTime + WarmupTime - GetServerTimer());
			if (CountdownInt != SecondsLeft)
			{
				gameTime = MatchTime + WarmupTime - GetServerTimer();
				SetHUDCountDown(gameTime);
				if(gameTime < 0.f && EGameState::End != BridgeState->GetGameState())  // 만약 제한시간이 종료되었다면 VR 플레이어 승리
				{
					BridgeState->GameWinner(EWinner::VR);
					BridgeState->GameMatchState(EGameState::End);
				}
			}

			CountdownInt = SecondsLeft;
		}
		else if(BridgeState->GetGameState() == EGameState::End)
		{
			SetHUDCountDown(0.f);
		}
	}
	
	

	
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
	// 게임이 끝났다면 menu 위젯 띄우지 않기..
	if(BridgeState->GetGameState() == EGameState::End) return;
	
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

void ABaseCharacterController::Respawn(ABaseCharacter* player)
{
	
	if(HasAuthority())
	{
		AProject_CrossBridgeGameModeBase* GM = Cast<AProject_CrossBridgeGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GM != nullptr)
		{
			
			if(player != nullptr)
			{
				UE_LOG(LogTemp,Warning,TEXT(" Restart Player "));
				GM->RestartPlayer(this);
			}
		}
	}
	
}

void ABaseCharacterController::SetHealthStatus(float curHP, float maxHP)
{
	PCHUD = PCHUD == nullptr ? Cast<AWeaponHUD>(GetHUD()) : PCHUD;


	bool bPCHUDValid = PCHUD && PCHUD->CharacterOverlay;

	if(bPCHUDValid)
	{
		
		PCHUD->CharacterOverlay->SetHealthBar(curHP,maxHP);
	}
	
}

void ABaseCharacterController::SetJetpackStatus(float curFuel, float maxFuel)
{
	PCHUD = PCHUD == nullptr ? Cast<AWeaponHUD>(GetHUD()) : PCHUD;


	bool bPCHUDValid = PCHUD && PCHUD->CharacterOverlay;

	if(bPCHUDValid)
	{
		
		PCHUD->CharacterOverlay->SetFuelBar(curFuel,maxFuel);
	}
}



