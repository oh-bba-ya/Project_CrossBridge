// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/Widget/BulletTestWidget.h"
#include "Components/Image.h"

UBulletTestWidget::UBulletTestWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

	//BulletTestSuccess1->SetVisibility(ESlateVisibility::Hidden);
	//BulletTestSuccess2->SetVisibility(ESlateVisibility::Hidden);
	//BulletTestSuccess3->SetVisibility(ESlateVisibility::Hidden);
}

void UBulletTestWidget::SetImageVisibility(int32 Num)
{
	if (Num == 1)
	{
		BulletTestSuccess1->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Num == 2)
	{
		BulletTestSuccess2->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Num == 3)
	{
		BulletTestSuccess3->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SuccessImage->SetVisibility(ESlateVisibility::Visible);
	}
}