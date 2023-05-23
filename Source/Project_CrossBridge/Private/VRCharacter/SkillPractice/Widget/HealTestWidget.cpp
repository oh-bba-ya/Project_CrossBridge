// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/Widget/HealTestWidget.h"
#include "Components/Image.h"

void UHealTestWidget::SetImageVisibility(int32 Num)
{
	if (Num == 1)
	{
		HealTestSuccess1->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Num == 2)
	{
		HealTestSuccess2->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Num == 3)
	{
		SuccessImage->SetVisibility(ESlateVisibility::Visible);
	}
}