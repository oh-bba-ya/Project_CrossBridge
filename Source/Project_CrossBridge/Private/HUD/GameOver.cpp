// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameOver.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameModeBase.h"
#include "Project_CrossBridge/Project_CrossBridgeGameModeBase.h"


void UGameOver::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if(World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if(PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if(QuitButton && !QuitButton->OnClicked.IsBound())
	{
		QuitButton->OnClicked.AddDynamic(this,&UGameOver::QuitButtonClicked);
	}

	if(ReStartButton && !ReStartButton->OnClicked.IsBound())
	{
		ReStartButton->OnClicked.AddDynamic(this,&UGameOver::ReStartButtonClicked);
	}
	
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if(MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UGameOver::OnDestroySession);
		}
	}
}


bool UGameOver::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	

	return true;
}

void UGameOver::OnDestroySession(bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		QuitButton->SetIsEnabled(true);
		return ;
	}
	
	UWorld* World = GetWorld();
	if(World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if(GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if(PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
		
	}
}

void UGameOver::QuitButtonClicked()
{
	QuitButton->SetIsEnabled(false);
	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
		MenuTearDown();
	}
}

void UGameOver::ReStartButtonClicked()
{

}


void UGameOver::MenuTearDown()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if(World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if(PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

	if(QuitButton && QuitButton->OnClicked.IsBound())
	{
		QuitButton->OnClicked.RemoveDynamic(this,&UGameOver::QuitButtonClicked);
	}

	if(MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UGameOver::OnDestroySession);
	}
}

void UGameOver::SetDisplayText(FString TextToDisplay)
{
	if(GameResultText)
	{
		GameResultText->SetText(FText::FromString(TextToDisplay));
	}
}
