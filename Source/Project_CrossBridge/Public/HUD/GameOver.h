// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UGameOver : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuSetup();
	void MenuTearDown();

	void SetDisplayText(FString TextToDisplay);

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

private:
	UPROPERTY(meta=(BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* ReStartButton;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* GameResultText;

	UFUNCTION()
	void QuitButtonClicked();

	UFUNCTION()
	void ReStartButtonClicked();

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY()
	bool bIsFirst = false;
	
};
