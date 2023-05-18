// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossBridgeState.h"

bool ACrossBridgeState::BpGetStart()
{
	return bIsStart;
}

void ACrossBridgeState::BPSetStart(bool b)
{
	bIsStart = b;
}
