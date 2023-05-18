// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossBridgeStateBase.h"

#include "Net/UnrealNetwork.h"

bool ACrossBridgeStateBase::BpGetStart()
{
	return bIsStart;
}

void ACrossBridgeStateBase::BPSetStart(bool b)
{
	bIsStart = b;
}


void ACrossBridgeStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACrossBridgeStateBase, OffsetTime);

}