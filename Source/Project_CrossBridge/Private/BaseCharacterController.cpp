// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterController.h"

#include "HUD/BaseCharacterWidget.h"
#include "HUD/ReturnToMainMenu.h"

void ABaseCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if(baseCharacterWidget != nullptr && IsLocalController())
	{
		baseCharacterUI = CreateWidget<UBaseCharacterWidget>(this,baseCharacterWidget);

		if(baseCharacterUI)
		{
			baseCharacterUI->AddToViewport();
		}
	}
	
}

void ABaseCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(InputComponent == nullptr) return;

	InputComponent->BindAction("Quit", IE_Pressed, this, &ABaseCharacterController::ShowReturnToMainMenu);
	
	
}

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

