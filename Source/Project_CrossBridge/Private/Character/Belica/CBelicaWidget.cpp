// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Belica/CBelicaWidget.h"
#include "Components/ProgressBar.h"
#include "Net/UnrealNetwork.h"

void UCBelicaWidget::SetFuelBar(float curFuel, float Max)
{
	fuelBar->SetPercent(curFuel/Max);
}




