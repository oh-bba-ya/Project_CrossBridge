// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlackholeTestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UBlackholeTestWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UImage* BlackholeTestSuccess;
	UPROPERTY(meta = (BindWidget))
		class UImage* SuccessImage;

	UFUNCTION()
		void SetImageVisibility(int32 Num);
};
