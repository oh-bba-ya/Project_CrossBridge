// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Belica/BelicaPlayerController.h"

#include "Character/Belica/CBelicaWidget.h"

void ABelicaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(belicaWidget != nullptr && IsLocalController())
	{
		belicaUI = CreateWidget<UCBelicaWidget>(this,belicaWidget);

		if(belicaUI)
		{
			belicaUI->AddToViewport();
		}
	}
}

