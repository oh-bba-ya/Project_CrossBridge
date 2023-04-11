// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidget.h"

#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString TextToLocalDisplay)
{
	if(DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToLocalDisplay));
	}
	
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString localRole;
	switch (LocalRole)
	{
	case ENetRole::ROLE_Authority:
		localRole = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		localRole = FString("AutonomousProxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		localRole = FString("SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		localRole = FString("None");
		break;
	}

	/*
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString remoteRole;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		remoteRole = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		remoteRole = FString("AutonomousProxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		remoteRole = FString("SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		remoteRole = FString("None");
		break;
	}
	*/
	
	//FString RemoteRoleString = FString::Printf(TEXT("Remote Role : %s"),*remoteRole);
	FString LocalRoleString = FString::Printf(TEXT("Local Role : %s"),*localRole);

	SetDisplayText(LocalRoleString);
}

void UOverheadWidget::NativeDestruct(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::NativeDestruct();
}

