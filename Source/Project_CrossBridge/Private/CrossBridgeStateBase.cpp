// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossBridgeStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACrossBridgeStateBase::ACrossBridgeStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACrossBridgeStateBase::BeginPlay()
{
	Super::BeginPlay();
	state = EGameState::Wait;
	eWinner = EWinner::NONE;
}

void ACrossBridgeStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (state)
	{
	case EGameState::Wait :
		break;
	case EGameState::Start:
		StartStateCallBack();
		break;
	case EGameState::End:
		EndStateCallBack();
		break;
	default:
			break;
	}
}




void ACrossBridgeStateBase::GameMatchState(EGameState e)
{
	state = e;
	bStateCall =false;
}

void ACrossBridgeStateBase::GameWinner(EWinner e)
{
	state = EGameState::End;
	eWinner = e;
}

void ACrossBridgeStateBase::WaitStateCallBack()
{

	waitStateDelegate.Broadcast();
}

void ACrossBridgeStateBase::StartStateCallBack()
{
	if(bStateCall)
	{
		return;
	}
	UE_LOG(LogTemp,Warning,TEXT("Start Callback"));
	if(StartSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),StartSound);
	}
	startStateDelegate.Broadcast((AActor*)nullptr);
	bStateCall = true;
}

void ACrossBridgeStateBase::EndStateCallBack()
{
	if(bStateCall)
	{
		return;
	}

	UE_LOG(LogTemp,Warning,TEXT("End Callback"));
	endStateDelegate.Broadcast();
	bStateCall = true;
}

void ACrossBridgeStateBase::QuitStateCallBack()
{
	quitStateDelegate.Broadcast();
}



