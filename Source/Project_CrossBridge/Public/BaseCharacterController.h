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
	

public:
	UPROPERTY(EditAnywhere, Category="Settings|Widget")
	TSubclassOf<class UBaseCharacterWidget> baseCharacterWidget;

	FORCEINLINE UBaseCharacterWidget* GetBaseCharacterUI() {return baseCharacterUI;}




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
	
	
};
