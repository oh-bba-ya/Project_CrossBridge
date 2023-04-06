// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Belica/CBelicaWidget.h"

#include "Character/CBelica.h"
#include "Components/ProgressBar.h"



void UCBelicaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	belica = Cast<ACBelica>(GetOwningPlayerPawn());
	
	if(belica != nullptr)
	{
		SetHealthBar(belica->GetCurrentHP(),belica->GetMaxHP());
		SetFuelBar(belica->GetFuel(),belica->GetMaxFuel());
	}
	
}

void UCBelicaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(belica != nullptr)
	{
		SetHealthBar(belica->GetCurrentHP(),belica->GetMaxHP());
		SetFuelBar(belica->GetFuel(),belica->GetMaxFuel());
	}
	
}


void UCBelicaWidget::SetFuelBar(float curFuel, float Max)
{
	fuelBar->SetPercent(curFuel/Max);
}

void UCBelicaWidget::SetHealthBar(float curHP, float MaxHP)
{
	HealthBar->SetPercent(curHP/MaxHP);
}





