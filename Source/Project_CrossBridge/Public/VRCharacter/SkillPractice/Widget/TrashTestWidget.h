// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrashTestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UTrashTestWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:

	UPROPERTY(meta = (BindWidget))
		class UImage* TrashTestSuccess;
	UPROPERTY(meta = (BindWidget))
		class UImage* SuccessImage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TrashCountText;

	UFUNCTION()
		void SetImageVisibility(int32 Num);
	UFUNCTION()
		void SetTrashCountText(int32 Num);
	
};
