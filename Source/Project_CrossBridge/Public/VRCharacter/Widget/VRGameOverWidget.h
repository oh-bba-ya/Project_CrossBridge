// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UVRGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GameResult;

	UFUNCTION()
		void SetResult(FString ResultText);
};
