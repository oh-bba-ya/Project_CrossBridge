// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CBelica.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/GunCombatComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Test/TestBullet.h"
#include "Weapon/Weapon.h"


ACBelica::ACBelica()
{
	Combat = CreateDefaultSubobject<UGunCombatComponent>(TEXT("GunCombatComponent"));
	Combat->SetIsReplicated(true);

}

void ACBelica::BeginPlay()
{
	Super::BeginPlay();
	
	// 벨리카 HP 초기화
	if(HasAuthority())
	{
		SetCurrentHealth(MaxHP);
	}

	if(GetLocalRole() == ENetRole::ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green, FString("Authority!"), true, FVector2d(1.2f));

	}
	else if(GetLocalRole() == ENetRole::ROLE_AutonomousProxy){
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green, FString("Client!"), true, FVector2d(1.2f));
	}
	
}


void ACBelica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bJetPackActive)
	{
		AddMovementInput(FVector(0,0,1));
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

/** Health Get, Set, TakeDamage */
#pragma region Health Get, Set, TakeDamage
void ACBelica::SetCurrentHealth(float healthValue)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		CurrentHP = FMath::Clamp(healthValue,0.f,MaxHP);
		
	}
}

void ACBelica::PlusHealth(int32 value)
{
	CurrentHP = FMath::Clamp(CurrentHP + value, 0.f,MaxHP);
}

void ACBelica::SubTractHealth(int32 value)
{
	CurrentHP = FMath::Clamp(CurrentHP - value, 0.f,MaxHP);
}

void ACBelica::Server_TakeDamage_Implementation(float value)
{
	SubTractHealth(value);
}

#pragma endregion 


#pragma region JetPack()
void ACBelica::FillUpFuel()
{
	if(Fuel >= MaxFuel)
	{
		GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
		Fuel = MaxFuel;
	}
	Fuel += FuelConsumptionSpeed;
	
}

void ACBelica::FuelConsumption(float value)
{
	Fuel = FMath::Clamp(Fuel - value,0,MaxFuel);
}


void ACBelica::ActivateJetPack()
{
	Server_ActivateJetPack();
}

void ACBelica::Server_ActivateJetPack_Implementation()
{
	bJetPackActive = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->AirControl = 1.f;

	FuelConsumption(FuelConsumptionSpeed);
	GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
	
}

void ACBelica::DeActivateJetPack()
{
	Server_DeActivateJetPack();
}

void ACBelica::Server_DeActivateJetPack_Implementation()
{
	bJetPackActive = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetCharacterMovement()->AirControl= 0.2f;
	GetWorld()->GetTimerManager().SetTimer(fuelTimer,this, &ACBelica::FillUpFuel, FuelRechargeSpeed,true,FuelRechargeDelay);
}
#pragma endregion 

#pragma region Funcions Aim
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
	if(Combat)
	{
		Combat->SetAiming(true);
	}
}

void ACBelica::AimEnd()
{
	if(Combat)
	{
		Combat->SetAiming(false);
	}
}

bool ACBelica::IsAiming()
{
	return (Combat && Combat->bAiming);
}
#pragma endregion 


void ACBelica::Attack()
{
	Server_Attack();
	if(Combat != nullptr)
	{
		Combat->FireButtonPressed(true);
	}
}

void ACBelica::Multicast_Attack_Implementation()
{
	if(FireMontage != nullptr)
	{
		PlayAnimMontage(FireMontage);
	}
}

void ACBelica::Server_Attack_Implementation()
{

	Multicast_Attack();

}

/** 무기 줍기 */
#pragma region Weapon Overlapping 
void ACBelica::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if(LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ACBelica::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;

	
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}
#pragma endregion 


// 서버에 복제 등록하기 위한 함수
void ACBelica::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACBelica,bJetPackActive);
	DOREPLIFETIME(ACBelica, Fuel);
	DOREPLIFETIME(ACBelica, CurrentHP);

	// 클라이언트에서만 위젯이 활성화 될 수 있도록 변경
	DOREPLIFETIME_CONDITION(ACBelica, OverlappingWeapon, COND_OwnerOnly);
	
}