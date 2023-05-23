// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BulletTestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UBulletTestWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UBulletTestWidget(const FObjectInitializer& ObjectInitializer);

public:

	UPROPERTY(meta = (BindWidget))
	class UImage* BulletTestSuccess1;	
	UPROPERTY(meta = (BindWidget))
	class UImage* BulletTestSuccess2;	
	UPROPERTY(meta = (BindWidget))
	class UImage* BulletTestSuccess3;
	UPROPERTY(meta = (BindWidget))
		class UImage* SuccessImage;

	UFUNCTION()
		void SetImageVisibility(int32 Num);
	
};
