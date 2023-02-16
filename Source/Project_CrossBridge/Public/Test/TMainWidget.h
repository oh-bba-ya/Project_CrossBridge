// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UTMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* btn_Start;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* btn_Quit;

	UFUNCTION()
		void StartGame();

	UFUNCTION()
		void QuitGame();

	
};
