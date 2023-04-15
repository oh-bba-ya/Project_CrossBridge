// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "BaseCharacterController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/WeaponHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Projectile.h"
#include "Weapon/ProjectileWeapon.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetupAttachment(springArmComp);

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Health 초기화.
	if(HasAuthority())
	{
		SetCurrentHealth(MaxHP);
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(BaseContext, 0);
		}
	}

	
}



// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bJetPackActive)
	{
		AddMovementInput(FVector(0,0,1));
	}

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(InputMovementAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		EnhancedInputComponent->BindAction(InputJumpAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Jump);
		EnhancedInputComponent->BindAction(InputJumpAction, ETriggerEvent::Completed, this, &ABaseCharacter::Release_Jump);
		EnhancedInputComponent->BindAction(InputAttackAction, ETriggerEvent::Started, this, &ABaseCharacter::Attack);
		EnhancedInputComponent->BindAction(InputContextualAction, ETriggerEvent::Started, this, &ABaseCharacter::ContextualActionPressed);
		EnhancedInputComponent->BindAction(InputContextualAction, ETriggerEvent::Completed, this, &ABaseCharacter::ContextualActionReleased);
		EnhancedInputComponent->BindAction(InputDropWeaponAction, ETriggerEvent::Started,this,&ABaseCharacter::DropWeapon);
		

	}

}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();


	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
	
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ABaseCharacter::Jump()
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

void ABaseCharacter::Attack()
{
	UE_LOG(LogTemp,Warning,TEXT("Base Attack"));
	Fire();
}

void ABaseCharacter::ContextualActionPressed()
{
	UE_LOG(LogTemp,Warning,TEXT("Base ContextualAction Pressed"));
}

void ABaseCharacter::ContextualActionReleased()
{
	UE_LOG(LogTemp,Warning,TEXT("Base ContextualAction Released"));
}



/** JetPack */
#pragma region JetPack()

void ABaseCharacter::Release_Jump()
{
	DeActivateJetPack();
}

void ABaseCharacter::FillUpFuel()
{
	if(Fuel >= MaxFuel)
	{
		GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
		Fuel = MaxFuel;
	}
	Fuel += FuelConsumptionSpeed;
	
}

void ABaseCharacter::FuelConsumption(float value)
{
	Fuel = FMath::Clamp(Fuel - value,0,MaxFuel);
}

void ABaseCharacter::ActivateJetPack()
{
	Server_ActivateJetPack();
}

void ABaseCharacter::Server_ActivateJetPack_Implementation()
{
	bJetPackActive = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->AirControl = 1.f;

	FuelConsumption(FuelConsumptionSpeed);
	GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
}


void ABaseCharacter::DeActivateJetPack()
{
	Server_DeActivateJetPack();
}

void ABaseCharacter::Server_DeActivateJetPack_Implementation()
{
	bJetPackActive = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetCharacterMovement()->AirControl= 0.2f;
	GetWorld()->GetTimerManager().SetTimer(fuelTimer,this, &ABaseCharacter::FillUpFuel, FuelRechargeSpeed,true,FuelRechargeDelay);
}

#pragma endregion

/** Health() */
#pragma region Health()
void ABaseCharacter::SetCurrentHealth(float healthValue)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		CurrentHP = FMath::Clamp(healthValue,0.f,MaxHP);
		
	}
}

void ABaseCharacter::PlusHealth(int32 value)
{
	CurrentHP = FMath::Clamp(CurrentHP + value, 0.f,MaxHP);
}

void ABaseCharacter::SubTractHealth(int32 value)
{
	CurrentHP = FMath::Clamp(CurrentHP - value, 0.f,MaxHP);
}

#pragma endregion






/** Fire */
#pragma region Fire()

void ABaseCharacter::Fire()
{
	if(myWeapon != nullptr)
	{
		myWeapon->Fire(this);
	}
}

void ABaseCharacter::Multicast_Fire_Implementation()
{
	if(fireMontage !=nullptr)
	{
		PlayAnimMontage(fireMontage);
	}
}
#pragma endregion


/** Weapon */
void ABaseCharacter::DropWeapon()
{
	if(GetController() != nullptr && GetController()->IsLocalController() && myWeapon != nullptr)
	{
		myWeapon->DropWeapon(this);
		HideCrosshair();
	}
}

void ABaseCharacter::HideCrosshair()
{
	ABaseCharacterController* controll = Cast<ABaseCharacterController>(GetController());
	if(Controller)
	{
		AWeaponHUD* HUD = Cast<AWeaponHUD>(controll->GetHUD());
		if(HUD)
		{
			FHUDStruct HudStruct;
			HudStruct.CrosshairCenter = nullptr;
			HudStruct.CrosshairRight = nullptr;
			HudStruct.CrosshairLeft = nullptr;
			HudStruct.CrosshairBottom = nullptr;
			HudStruct.CrosshairTop = nullptr;
			HUD->SetHUDStruct(HudStruct);
		}
	}
	
}


// 서버에 복제 등록하기 위한 함수
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABaseCharacter,bJetPackActive);
	DOREPLIFETIME(ABaseCharacter, Fuel);
	DOREPLIFETIME(ABaseCharacter, CurrentHP);
	
}


