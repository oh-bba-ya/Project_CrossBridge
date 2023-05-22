// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/Widget/SwordTestWidget.h"
#include "Components/Image.h"

void USwordTestWidget::SetImageVisibility(int32 Num)
{
	if (Num == 1)
	{
		SwordTestSuccess->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Num == 2)
	{
		SuccessImage->SetVisibility(ESlateVisibility::Visible);
	}

}