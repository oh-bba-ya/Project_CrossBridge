// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TMainWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTMainWidget::NativeConstruct() {
	btn_Start->OnClicked.AddDynamic(this, &UTMainWidget::StartGame);
	btn_Quit->OnClicked.AddDynamic(this, &UTMainWidget::QuitGame);
}

void UTMainWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("SelectionLevel"));
}

void UTMainWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
