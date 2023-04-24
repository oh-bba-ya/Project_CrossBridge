// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UVRStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HPText;
	
	void SetHPBar(float Percent);

};
