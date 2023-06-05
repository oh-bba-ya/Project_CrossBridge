// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/Widget/VRGameOverWidget.h"
#include "Components/TextBlock.h"

void UVRGameOverWidget::SetResult(FString ResultText)
{
	GameResult->SetText(FText::FromString(ResultText));
}