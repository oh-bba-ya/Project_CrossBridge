// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/Widget/BlackholeTestWidget.h"
#include "Components/Image.h"

void UBlackholeTestWidget::SetImageVisibility(int32 Num)
{
	if (Num == 1)
	{
		BlackholeTestSuccess->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Num == 2)
	{
		SuccessImage->SetVisibility(ESlateVisibility::Visible);
	}
}