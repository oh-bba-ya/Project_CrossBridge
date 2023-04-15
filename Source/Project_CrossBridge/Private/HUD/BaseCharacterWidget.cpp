// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BaseCharacterWidget.h"
#include "Character/BaseCharacter.h"
#include "Components/ProgressBar.h"

void UBaseCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	baseCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	
	if(baseCharacter != nullptr)
	{
		SetHealthBar(baseCharacter->GetCurrentHP(),baseCharacter->GetMaxHP());
		SetFuelBar(baseCharacter->GetFuel(),baseCharacter->GetMaxFuel());
	}
	
}

void UBaseCharacterWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(baseCharacter != nullptr)
	{
		SetHealthBar(baseCharacter->GetCurrentHP(),baseCharacter->GetMaxHP());
		SetFuelBar(baseCharacter->GetFuel(),baseCharacter->GetMaxFuel());
	}
	
}


void UBaseCharacterWidget::SetFuelBar(float curFuel, float Max)
{
	fuelBar->SetPercent(curFuel/Max);
}

void UBaseCharacterWidget::SetHealthBar(float curHP, float MaxHP)
{
	HealthBar->SetPercent(curHP/MaxHP);
}
