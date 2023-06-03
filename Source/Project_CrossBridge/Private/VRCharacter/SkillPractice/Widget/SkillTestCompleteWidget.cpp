// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/Widget/SkillTestCompleteWidget.h"
#include "Components/Image.h"
#include "VRCharacter/SkillPractice/Portal.h"
#include "Kismet/GameplayStatics.h"

void USkillTestCompleteWidget::SetImageVisibility(int32 Num)
{
	switch(Num)
	{
	case 1:
		Clear1->SetVisibility(ESlateVisibility::Visible);
		break;
		
	case 2:
		Clear2->SetVisibility(ESlateVisibility::Visible);
		break;

	case 3:
		Clear3->SetVisibility(ESlateVisibility::Visible);
		break;

	case 4:
		Clear4->SetVisibility(ESlateVisibility::Visible);
		break;

	case 5:
		Clear5->SetVisibility(ESlateVisibility::Visible);
		break;

	case 6:
		Clear6->SetVisibility(ESlateVisibility::Visible);
		break;

	case 7:
		Clear7->SetVisibility(ESlateVisibility::Visible);
		break;

	case 8:
		Clear8->SetVisibility(ESlateVisibility::Visible);
		class APortal* Portal = Cast<APortal>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal::StaticClass()));
		Portal->EndPortalVisible();
		break;
	}
}