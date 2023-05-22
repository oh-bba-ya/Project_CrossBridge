// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/Widget/TrashTestWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UTrashTestWidget::SetImageVisibility(int32 Num)
{
	
	if (Num == 1)
	{
		TrashTestSuccess->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Num == 2)
	{
		SuccessImage->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void UTrashTestWidget::SetTrashCountText(int32 Num)
{
	TrashCountText->SetText(UKismetTextLibrary::Conv_IntToText(Num));
}