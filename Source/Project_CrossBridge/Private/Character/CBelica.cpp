// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CBelica.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Belica/CBelicaWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"


ACBelica::ACBelica()
{
	
}

void ACBelica::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner())
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green, FString("Server!"), true, FVector2d(1.2f));

	}

	BelicaUI = CreateWidget<UCBelicaWidget>(GetWorld(),BelicaWidget);

	if(BelicaUI != nullptr)
	{
		BelicaUI->AddToViewport();
	}
}

void ACBelica::Jump()
{
	if(Fuel > 0)
	{
		ActivateJetPack();
	}
	else
	{
		DeActivateJetPack();
	}

}

void ACBelica::Release_Jump()
{
	DeActivateJetPack();
}

void ACBelica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bJetPackActive)
	{
		AddMovementInput(FVector(0,0,1));
	}
	
}

void ACBelica::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(InputJumpAction, ETriggerEvent::Completed, this, &ACBelica::Release_Jump);
	}
}

void ACBelica::FillUpFuel()
{
	if(Fuel >= MaxFuel)
	{
		GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
		Fuel = MaxFuel;
	}
	Fuel += FuelConsumption;
	UE_LOG(LogTemp,Warning,TEXT("Fuel : %.2f"),Fuel);

	BelicaUI->SetFuelBar(Fuel,MaxFuel);
}

void ACBelica::ActivateJetPack()
{
	//ServerActivateJetPack();
	
	bJetPackActive = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->AirControl = 1.f;

	Fuel -= FuelConsumption;
	BelicaUI->SetFuelBar(Fuel,MaxFuel);
	UE_LOG(LogTemp,Warning,TEXT("Fuel : %.2f"),Fuel);
	GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
	
}

void ACBelica::DeActivateJetPack()
{
	
	//ServerDeActivateJetPack();
	bJetPackActive = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetCharacterMovement()->AirControl= 0.2f;
	GetWorld()->GetTimerManager().SetTimer(fuelTimer,this, &ACBelica::FillUpFuel, FuelRechargeSpeed,true,FuelRechargeDelay);
	
}

void ACBelica::ServerActivateJetPack_Implementation()
{
	bJetPackActive = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->AirControl = 1.f;

	Fuel -= FuelConsumption;
	BelicaUI->SetFuelBar(Fuel,MaxFuel);
	UE_LOG(LogTemp,Warning,TEXT("Fuel : %.2f"),Fuel);
	GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
}

void ACBelica::ServerDeActivateJetPack_Implementation()
{
	bJetPackActive = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetCharacterMovement()->AirControl= 0.2f;
	GetWorld()->GetTimerManager().SetTimer(fuelTimer,this, &ACBelica::FillUpFuel, FuelRechargeSpeed,true,FuelRechargeDelay);
}


