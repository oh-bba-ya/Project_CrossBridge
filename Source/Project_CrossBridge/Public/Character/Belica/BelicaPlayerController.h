// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BelicaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ABelicaPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCBelicaWidget> belicaWidget;

	FORCEINLINE UCBelicaWidget* GetBelicaUI() {return belicaUI;}

private:
	UCBelicaWidget* belicaUI;

	
};
