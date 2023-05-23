// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTestCompleteWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API USkillTestCompleteWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
		class UImage* Clear1;
	UPROPERTY(meta = (BindWidget))
		class UImage* Clear2;
	UPROPERTY(meta = (BindWidget))
		class UImage* Clear3;
	UPROPERTY(meta = (BindWidget))
		class UImage* Clear4;
	UPROPERTY(meta = (BindWidget))
		class UImage* Clear5;
	UPROPERTY(meta = (BindWidget))
		class UImage* Clear6;
	UPROPERTY(meta = (BindWidget))
		class UImage* Clear7;
	UPROPERTY(meta = (BindWidget))
		class UImage* Clear8;

	UFUNCTION()
		void SetImageVisibility(int32 Num);

	UPROPERTY()
		int32 Index;
};
