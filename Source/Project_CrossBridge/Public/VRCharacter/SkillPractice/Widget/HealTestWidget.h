// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealTestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UHealTestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
		class UImage* HealTestSuccess1;
	UPROPERTY(meta = (BindWidget))
		class UImage* HealTestSuccess2;
	UPROPERTY(meta = (BindWidget))
		class UImage* SuccessImage;

	UFUNCTION()
		void SetImageVisibility(int32 Num);
};
