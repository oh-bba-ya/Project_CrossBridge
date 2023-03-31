// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GunCombatComponent.h"

#include "Character/CBelica.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGunCombatComponent::UGunCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGunCombatComponent::SetAiming(bool bisAiming)
{
	bAiming = bisAiming;
	ServerSetAiming(bisAiming);
}

void UGunCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}


// Called every frame
void UGunCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGunCombatComponent, bAiming);
}


