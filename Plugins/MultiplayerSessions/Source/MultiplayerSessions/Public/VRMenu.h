// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "VRMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UVRMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void VRMenuSetup(int32 NumberOfPublicConnections = 10, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/Maps/Bridge")));

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* editText_id;
	
	UFUNCTION()
	void ClickStart();
	
protected:
	void NativeConstruct() override;
	bool Initialize() override;

	void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION(BlueprintCallable)
	void JoinButtonClicked();

	void MenuTearDown();

	// The Subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	
	int32 NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("") };
	
};
