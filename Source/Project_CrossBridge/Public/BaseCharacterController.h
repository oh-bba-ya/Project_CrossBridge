// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ABaseCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual float GetServerTimer();
	virtual void ReceivedPlayer() override;
	

public:
	UPROPERTY(EditAnywhere, Category="Settings|Widget")
	TSubclassOf<class UBaseCharacterWidget> baseCharacterWidget;

	FORCEINLINE UBaseCharacterWidget* GetBaseCharacterUI() {return baseCharacterUI;}

/** MatchTime*/
	void SetHUDCountDown(float CountdownTime);
	void SetHUDTime();

	/** Sync time between client and server */
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClinetRequest);

	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f;

	UPROPERTY(EditAnywhere, Category = "Settings|Time")
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float DeltaTime);



private:
	UBaseCharacterWidget* baseCharacterUI;

	/** Quit Widget */
protected:
	void ShowReturnToMainMenu();

private:
	/**
	 * return to main menu
	 */

	UPROPERTY(EditAnywhere, Category=HUD)
	TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
	class UReturnToMainMenu* ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;

	UPROPERTY(EditAnywhere, Category = "Settings|Time")
	float MatchTime = 120.f;
	float CountdownInt = 0.f;
	
};
