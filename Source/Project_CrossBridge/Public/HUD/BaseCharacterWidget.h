// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UBaseCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Fuel", meta = (BindWidget))
	class UProgressBar* fuelBar;

	
	void SetFuelBar(float curFuel, float Max);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|HealthBar", meta = (BindWidget))
	class UProgressBar* HealthBar;

	void SetHealthBar(float curHP, float MaxHP);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountdownText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Fuel", meta = (BindWidget))
	class UProgressBar* freezeBar;

	void SetFreezeBar(float cur,float max);


	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



	
};
