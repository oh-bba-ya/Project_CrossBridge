// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterController.h"

#include "HUD/BaseCharacterWidget.h"

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

