// Fill out your copyright notice in the Description page of Project Settings.


#include "VRMenu.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UVRMenu::VRMenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"),*LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);

		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("VRMenuSetup")));
	}
}

void UVRMenu::ClickStart()
{
	// 만일, ID가 빈칸이 아니라면 0번-> 1번 캔버스로 변경
	if(!editText_id->GetText().IsEmpty())
	{
		if(MultiplayerSessionsSubsystem != nullptr)
		{
			MultiplayerSessionsSubsystem->SessionID = FName(*editText_id->GetText().ToString());
			JoinButtonClicked();
		}
	}
}

void UVRMenu::NativeConstruct()
{
	editText_id->SetText(FText::FromString("VR"));
}

bool UVRMenu::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}

	if (JoinButton) {
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::ClickStart);
	}

	return true;
}

void UVRMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void UVRMenu::OnCreateSession(bool bWasSuccessful)
{
	/* VR은 현재 Host를 만들 수 없기 때문에 주석처리..
	if (bWasSuccessful) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("Session created Successfully!")));
		}

		UWorld* World = GetWorld();
		if (World) {
			World->ServerTravel(PathToLobby);
		}
	}
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed to created session!")));
		}
		HostButton->SetIsEnabled(true);
	}
	*/
}

void UVRMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr) {
		return;
	}

	for (auto Result : SessionResult) {
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType) {
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	

	if (!bWasSuccessful || SessionResult.Num() == 0) {
		JoinButton->SetIsEnabled(true);
	}
}

void UVRMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);


			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController) {
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success) {
		JoinButton->SetIsEnabled(true);
	}
}

void UVRMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UVRMenu::OnStartSession(bool bWasSuccessful)
{
}

void UVRMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->FindSessions(10000);
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("ClickJoinButtonFindSession")));
	}
}

void UVRMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);

		}
	}
}


