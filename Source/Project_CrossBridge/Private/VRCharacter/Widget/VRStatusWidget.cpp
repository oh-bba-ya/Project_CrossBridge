// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/Widget/VRStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/Image.h"
#include "BaseCharacterController.h"


void UVRStatusWidget::SetHPBar(float Percent)
{
	float Rate = 1 - Percent;
	Rate = Rate >= 0 ? Rate : 0;
	HPBar->SetPercent(Rate);
	HPText->SetText(UKismetTextLibrary::Conv_IntToText((Rate) * 100));
}

void UVRStatusWidget::SetCooltimeText(FString Weapon, float Time)
{
	if (Weapon == "Blackhole")
	{
		if (Time == 0)
		{
			BlackholeCool->SetText(FText::FromString(TEXT("")));
		}
		else
		{ 
			BlackholeCool->SetText(UKismetTextLibrary::Conv_IntToText(Time));
		}
	}
	else if (Weapon == "Trash")
	{
		if (Time == 0)
		{
			TrashCool->SetText(FText::FromString(TEXT("")));
		}
		else
		{
			TrashCool->SetText(UKismetTextLibrary::Conv_IntToText(Time));
		}
	}
	else if (Weapon == "Sword")
	{
		if (Time == 0)
		{
			SwordCool->SetText(FText::FromString(TEXT("")));
		}
		else
		{
			SwordCool->SetText(UKismetTextLibrary::Conv_IntToText(Time));
		}
	}
}

void UVRStatusWidget::SetImageColor(FString Weapon, bool IsCoolTime)
{
	if (Weapon == "Blackhole")
	{
		if (IsCoolTime)
		{
			BlackholeImage->SetBrushTintColor(UnavailableColor);
		}
		else
		{
			BlackholeImage->SetBrushTintColor(AvailableColor);
		}
	}
	else if (Weapon == "Trash")
	{
		if (IsCoolTime)
		{
			TrashImage->SetBrushTintColor(UnavailableColor);
		}
		else
		{
			TrashImage->SetBrushTintColor(AvailableColor);
		}
	}
	else if (Weapon == "Sword")
	{
		if (IsCoolTime)
		{
			SwordImage->SetBrushTintColor(UnavailableColor);
		}
		else
		{
			SwordImage->SetBrushTintColor(AvailableColor);
		}
	}
}

void UVRStatusWidget::SetTimerText(FString GameTimer)
{
	GameTime->SetText(FText::FromString(GameTimer));
}