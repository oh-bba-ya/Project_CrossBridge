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

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* BlackholeCool;

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* ThunderCool;

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* SwordCool;

		UPROPERTY(meta = (BindWidget))
		class UImage* BlackholeImage;

		UPROPERTY(meta = (BindWidget))
		class UImage* ThunderImage;

		UPROPERTY(meta = (BindWidget))
		class UImage* SwordImage;

		UPROPERTY()
		FLinearColor AvailableColor = (FLinearColor)FVector(1);

		UPROPERTY()
		FLinearColor UnavailableColor = (FLinearColor)FVector(0.3f);
	
	void SetHPBar(float Percent);

	void SetCooltimeText(FString Weapon, float Time);

	void SetImageColor(FString Weapon, bool IsCoolTime);

};
