// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CBelica.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Belica/CBelicaWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/GunCombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Test/TestBullet.h"


ACBelica::ACBelica()
{
	Combat = CreateDefaultSubobject<UGunCombatComponent>(TEXT("GunCombatComponent"));
	Combat->SetIsReplicated(true);
}

void ACBelica::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ENetRole::ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green, FString("Authority!"), true, FVector2d(1.2f));

	}
	else if(GetLocalRole() == ENetRole::ROLE_AutonomousProxy){
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green, FString("Client!"), true, FVector2d(1.2f));
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
		ServerActivateJetPack();
	}
	else
	{
		ServerDeActivateJetPack();
	}

}

void ACBelica::Release_Jump()
{
	ServerDeActivateJetPack();
}

void ACBelica::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//auto land = OtherComp->GetCollisionObjectType();
	
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

void ACBelica::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->Character = this;
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

	BelicaUI->SetFuelBar(Fuel,MaxFuel);
}

void ACBelica::ServerFillUpFuel_Implementation()
{
	if(Fuel >= MaxFuel)
	{
		GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
		Fuel = MaxFuel;
	}
	Fuel += FuelConsumption;

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

void ACBelica::ContextualActionPressed()
{
	AimStart();
}

void ACBelica::ContextualActionReleased()
{
	AimEnd();
}

void ACBelica::AimStart()
{
	UE_LOG(LogTemp,Warning,TEXT("Pressed"));
	if(Combat)
	{
		Combat->SetAiming(true);
	}
}

void ACBelica::AimEnd()
{
	UE_LOG(LogTemp,Warning,TEXT("Released"));
	if(Combat)
	{
		Combat->SetAiming(false);
	}
}

bool ACBelica::IsAiming()
{
	return (Combat && Combat->bAiming);
}


void ACBelica::Attack()
{
	ServerAttack();
}

void ACBelica::MulticastAttack_Implementation()
{
	if(FireMontage != nullptr)
	{
		PlayAnimMontage(FireMontage);
	}
}

void ACBelica::ServerAttack_Implementation()
{

	GetWorld()->SpawnActor<ATestBullet>
	(bulletFactory, GetActorLocation() + GetActorForwardVector()*150,GetActorRotation());


	MulticastAttack();

}

void ACBelica::ServerActivateJetPack_Implementation()
{
	bJetPackActive = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->AirControl = 1.f;

	Fuel -= FuelConsumption;
	BelicaUI->SetFuelBar(Fuel,MaxFuel);
	GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
}

void ACBelica::ServerDeActivateJetPack_Implementation()
{
	bJetPackActive = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetCharacterMovement()->AirControl= 0.2f;
	GetWorld()->GetTimerManager().SetTimer(fuelTimer,this, &ACBelica::FillUpFuel, FuelRechargeSpeed,true,FuelRechargeDelay);
}

// 서버에 복제 등록하기 위한 함수
void ACBelica::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACBelica,bJetPackActive);
	/*
	DOREPLIFETIME(ACBelica,Fuel);
	DOREPLIFETIME(ACBelica,MaxFuel);
	DOREPLIFETIME(ACBelica,FuelConsumption);
	DOREPLIFETIME(ACBelica,FuelRechargeSpeed);
	DOREPLIFETIME(ACBelica,FuelRechargeDelay);
	*/
	
}