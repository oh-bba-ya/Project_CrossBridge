// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/Widget/VRStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UVRStatusWidget::SetHPBar(float Percent)
{
	float Rate = 1 - Percent;
	Rate = Rate >= 0 ? Rate : 0;
	HPBar->SetPercent(Rate);
	HPText->SetText(UKismetTextLibrary::Conv_IntToText((Rate) * 100));
}