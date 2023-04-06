// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CBelicaWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UCBelicaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Belica|Fuel", meta = (BindWidget))
	class UProgressBar* fuelBar;


	void SetFuelBar(float curFuel, float Max);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Belica|HealthBar", meta = (BindWidget))
	class UProgressBar* HealthBar;

	void SetHealthBar(float curHP, float MaxHP);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private:
	class ACBelica* belica;
};
