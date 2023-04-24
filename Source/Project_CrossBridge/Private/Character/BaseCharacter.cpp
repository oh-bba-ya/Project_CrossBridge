// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "BaseCharacterController.h"
#include "CrossPlayerState.h"
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
#include "Weapon/ProjectileWeapon.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Components/TextBlock.h"
#include "HUD/OverheadWidget.h"
#include "EnhancedInputSubsystemInterface.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Objects/BaseGrabbableActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects/Blackhole.h"
#include "Components/CapsuleComponent.h"
#include "Objects/ThrowingWeapon.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetupAttachment(springArmComp);

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(VRCamera);

	HeadComp = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadComp"));
	HeadComp->SetupAttachment(HeadMesh);
	HeadComp->SetRelativeLocation(FVector(0, 0, 170));
	HeadComp->SetBoxExtent(FVector(10, 10, 12));

	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));

	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));

	LeftGrip = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftGrip"));
	LeftGrip->SetupAttachment(RootComponent);
	LeftGrip->SetTrackingMotionSource(FName("LeftGrip"));

	LeftAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftAim"));
	LeftAim->SetupAttachment(RootComponent);
	LeftAim->SetTrackingMotionSource(FName("LeftAim"));

	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	RightAim->SetupAttachment(RootComponent);
	RightAim->SetTrackingMotionSource(FName("RightAim"));

	LeftHandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandBox"));
	LeftHandBox->SetupAttachment(LeftHand);
	LeftHandBox->SetRelativeLocation(FVector(3, 1, -4));
	LeftHandBox->SetBoxExtent(FVector(5));

	RightHandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandBox"));
	RightHandBox->SetupAttachment(RightHand);
	RightHandBox->SetRelativeLocation(FVector(3, -1, -4));
	RightHandBox->SetBoxExtent(FVector(5));

	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>VRHeadMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/VR_Head.VR_Head'"));
	if (VRHeadMesh.Succeeded())
	{
		HeadMesh->SetSkeletalMesh(VRHeadMesh.Object);
		HeadMesh->SetRelativeLocation(FVector(0, 0, -170));
		HeadMesh->SetRelativeRotation(FRotator(0, -90, 0));
		HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>LeftMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (LeftMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(LeftMesh.Object);
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
	}

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh>RightMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (RightMesh.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(RightMesh.Object);
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance>HandAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/MannequinsXR/Meshes/ABP_MannequinsXR.ABP_MannequinsXR_C'"));
	if (HandAnim.Succeeded())
	{
		LeftHandMesh->SetAnimInstanceClass(HandAnim.Class);
		RightHandMesh->SetAnimInstanceClass(HandAnim.Class);
	}

	LeftHandBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnLeftHandOverlap);
	LeftHandBox->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnLeftHandEndOverlap);
	RightHandBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnRightHandOverlap);
	RightHandBox->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnRightHandEndOverlap);

	
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
			Subsystem->AddMappingContext(IMC_VRInput, 0);
			Subsystem->AddMappingContext(IMC_VRHand, 0);
		}
	}


	// 내가 조종하는 캐릭터
	if(GetController() != nullptr && GetController()->IsLocalController())
	{
		// Player Name
		GameInstance = GetGameInstance();

		if(GameInstance != nullptr)
		{
			MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
			
			if(MultiplayerSessionsSubsystem)
			{
				ServerSetName(MultiplayerSessionsSubsystem->SessionID.ToString());
			}
		}
	}

	

	if(OverheadWidget)
	{
		overhead = Cast<UOverheadWidget>(OverheadWidget->GetWidget());
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (GetController() && GetController()->IsLocalController())
	{
		IsVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
		if (IsVR)
		{
			UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
			ServerVRSetting();

			RedDot = GetWorld()->SpawnActor<AActor>(SpawnRedDot, GetActorLocation(), GetActorRotation());
			RedDot->SetActorHiddenInGame(true);
			RedDot->SetActorEnableCollision(false);
			VRCurHP = VRMaxHP;
		}
		else
		{
			ServerPCSetting();
		}
	}

	UMaterialInterface* HeadBase = HeadMesh->GetMaterial(0);
	if (HeadBase)
	{
		HeadMat = HeadMesh->CreateDynamicMaterialInstance(0, HeadBase);
	}
	UMaterialInterface* LeftHandBase = LeftHandMesh->GetMaterial(0);
	if (LeftHandBase)
	{
		LeftHandMat = LeftHandMesh->CreateDynamicMaterialInstance(0, LeftHandBase);
	}
	UMaterialInterface* RightHandBase = RightHandMesh->GetMaterial(0);
	if (RightHandBase)
	{
		RightHandMat = RightHandMesh->CreateDynamicMaterialInstance(0, RightHandBase);
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(0, 0, 0));
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

	if(overhead)
	{
		overhead->DisplayText->SetText(FText::FromString(myName));
	}





	if (IsVR)
	{
		ServerVRTransform(VRCamera->GetRelativeTransform(), LeftHand->GetRelativeTransform(), RightHand->GetRelativeTransform());

	}
	
	SetGrabInfo();
	

	if (IsLeftY)
	{
		LeftYTimer += DeltaTime;
		if (LeftYTimer / LeftYCastTime <= 1)
		{

			//ColorChange(LeftYTimer / 5, FString("Left"));
			ServerColorChange(LeftYTimer / LeftYCastTime, FString("LeftY"));
			
		}

	}

	if (IsLeftX)
	{
		if (!Blackhole) 
		{
			Blackhole = Cast<ABlackhole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackhole::StaticClass()));
		}
		FVector BlackHoleLoc = BlackHoleTrace();
		LeftXTimer += DeltaTime;
		if (LeftXTimer / LeftXCastTime <= 1)
		{ 
			ServerColorChange(LeftXTimer / LeftXCastTime, FString("LeftX"));
			//Blackhole->ServerBlackholeSize(LeftXTimer / LeftXCastTime);
			ServerBlackholeSet(LeftXTimer / LeftXCastTime, BlackHoleLoc);
		}
		else
		{
			ServerBlackholeSet(1, BlackHoleLoc);
		}

	}
	 if (IsBlackholeSet)
	{
		if (BlackholeTimer == 0)
		{
			Blackhole->IsBlackholeActive = true;
		}
		BlackholeTimer += DeltaTime;
			
		if (BlackholeTimer / LeftXCastTime > 1)
		{
			ServerBlackholeActivate(false);
			ServerBlackholeReset();
		}
	}
	else if (!IsBlackholeSet && BlackholeTimer > 0)
	{
		BlackholeTimer = 0;
		Blackhole->IsBlackholeActive = false;
	}

	 if (IsRightAB)
	 {
		 RightABTimer += DeltaTime;
		 SetRedDot();
		 if (RightABTimer / RightABCastTime <= 1)
		 {
			 ServerColorChange(RightABTimer / RightABCastTime, FString("Right"));
		 }
	 }



	FVector StartPos = RightAim->GetComponentLocation();
	FVector EndPos = StartPos + RightAim->GetForwardVector() * 1000;
	//DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, -1, 0, 1);


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
		
		

		
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::VRMove);
		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ABaseCharacter::Turn);
		EnhancedInputComponent->BindAction(IA_LeftIndexCurl, ETriggerEvent::Triggered, this, &ABaseCharacter::LeftIndexCurl);
		EnhancedInputComponent->BindAction(IA_LeftGrasp, ETriggerEvent::Triggered, this, &ABaseCharacter::LeftGrasp);
		EnhancedInputComponent->BindAction(IA_LeftY, ETriggerEvent::Triggered, this, &ABaseCharacter::LeftY);
		EnhancedInputComponent->BindAction(IA_LeftX, ETriggerEvent::Triggered, this, &ABaseCharacter::LeftX);
		EnhancedInputComponent->BindAction(IA_RightIndexCurl, ETriggerEvent::Triggered, this, &ABaseCharacter::RightIndexCurl);
		EnhancedInputComponent->BindAction(IA_RightGrasp, ETriggerEvent::Triggered, this, &ABaseCharacter::RightGrasp);
		EnhancedInputComponent->BindAction(IA_RightB, ETriggerEvent::Triggered, this, &ABaseCharacter::RightB);
		EnhancedInputComponent->BindAction(IA_RightA, ETriggerEvent::Triggered, this, &ABaseCharacter::RightA);
		EnhancedInputComponent->BindAction(IA_LeftIndexCurl, ETriggerEvent::Completed, this, &ABaseCharacter::LeftIndexCurlEnd);
		EnhancedInputComponent->BindAction(IA_LeftGrasp, ETriggerEvent::Completed, this, &ABaseCharacter::LeftGraspEnd);
		EnhancedInputComponent->BindAction(IA_LeftY, ETriggerEvent::Completed, this, &ABaseCharacter::LeftYEnd);
		EnhancedInputComponent->BindAction(IA_LeftX, ETriggerEvent::Completed, this, &ABaseCharacter::LeftXEnd);
		EnhancedInputComponent->BindAction(IA_RightIndexCurl, ETriggerEvent::Completed, this, &ABaseCharacter::RightIndexCurlEnd);
		EnhancedInputComponent->BindAction(IA_RightGrasp, ETriggerEvent::Completed, this, &ABaseCharacter::RightGraspEnd);
		EnhancedInputComponent->BindAction(IA_RightB, ETriggerEvent::Completed, this, &ABaseCharacter::RightBEnd);
		EnhancedInputComponent->BindAction(IA_RightA, ETriggerEvent::Completed, this, &ABaseCharacter::RightAEnd);
		



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
	}
}



/** Player Name */
void ABaseCharacter::ServerSetName_Implementation(const FString& name)
{
	myName = name;
	
	ACrossPlayerState* ps = Cast<ACrossPlayerState>(GetPlayerState());

	if(ps!=nullptr)
	{
		ps->SetPlayerName(name);
	}
	
}



// 서버에 복제 등록하기 위한 함수
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABaseCharacter,bJetPackActive);
	DOREPLIFETIME(ABaseCharacter, Fuel);
	DOREPLIFETIME(ABaseCharacter, CurrentHP);
	DOREPLIFETIME(ABaseCharacter, myName);
	DOREPLIFETIME(ABaseCharacter, IsBlackholeSet);
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ABaseCharacter::VRMove(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);


}

void ABaseCharacter::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);

}



void ABaseCharacter::LeftIndexCurl()
{
	IsLeftIndexCurl = true;
	if (GrabbedActorLeft && IsLeftGrasp && !IsLeftGrab)
	{
		ServerGrabTheActor(GrabbedActorLeft, FString("Left"));
	}
}

void ABaseCharacter::LeftGrasp()
{
	IsLeftGrasp = true;
	if (GrabbedActorLeft && IsLeftIndexCurl && !IsLeftGrab)
	{
		ServerGrabTheActor(GrabbedActorLeft, FString("Left"));
	}
}

void ABaseCharacter::LeftY()
{
	if (!IsLeftIndexCurl && !IsLeftGrasp &&!IsLeftX)
	{
		IsLeftY = true;
	}
}
void ABaseCharacter::LeftYEnd()
{
	if (LeftYTimer / LeftYCastTime > 1)
	{
		ServerSpawnGrabbableActor();
	}
	IsLeftY = false;
	LeftYTimer = 0;
	//ResetColorChange(FString("Left"));
	ServerResetColorChange(FString("Left"));
}

void ABaseCharacter::LeftX()
{
	if (!IsLeftIndexCurl && !IsLeftGrasp && !IsLeftY && !IsBlackholeSet)
	{
		IsLeftX = true;
	}

}

void ABaseCharacter::LeftXEnd()
{
	if (LeftXTimer / LeftXCastTime > 1)
	{
		//Blackhole->ServerBlackholeReset();
		//ServerBlackholeReset();
	}
	
	ServerBlackholeActivate(true);
	
	IsLeftX = false;
	LeftXTimer = 0;
	ServerResetColorChange(FString("Left"));
}

void ABaseCharacter::RightIndexCurl()
{
	IsRightIndexCurl = true;
	if (GrabbedActorRight && IsRightGrasp && !IsRightGrab)
	{
		ServerGrabTheActor(GrabbedActorRight, FString("Right"));
		RightPrevLoc = RightHand->GetComponentLocation();
		RightPrevRot = RightHand->GetComponentQuat();
	}
}

void ABaseCharacter::RightGrasp()
{
	IsRightGrasp = true;
	if (GrabbedActorRight && IsRightIndexCurl && !IsRightGrab)
	{
		ServerGrabTheActor(GrabbedActorRight, FString("Right"));
		RightPrevLoc = RightHand->GetComponentLocation();
		RightPrevRot = RightHand->GetComponentQuat();
	}

}
void ABaseCharacter::LeftIndexCurlEnd()
{
	IsLeftIndexCurl = false;
	if (IsLeftGrab)
	{
		ServerUnGrabTheActor(GrabbedActorLeft, FString("Left"), RightThrowDir, RightThrowRot);
	}
}

void ABaseCharacter::LeftGraspEnd()
{
	IsLeftGrasp = false;
	if (IsLeftGrab)
	{
		ServerUnGrabTheActor(GrabbedActorLeft, FString("Left"), RightThrowDir, RightThrowRot);
	}
}

void ABaseCharacter::RightIndexCurlEnd()
{
	IsRightIndexCurl = false;
	if (IsRightGrab)
	{
		ServerUnGrabTheActor(GrabbedActorRight, FString("Right"), RightThrowDir, RightThrowRot);

	}
}

void ABaseCharacter::RightGraspEnd()
{
	IsRightGrasp = false;
	if (IsRightGrab)
	{
		ServerUnGrabTheActor(GrabbedActorRight, FString("Right"), RightThrowDir, RightThrowRot);

	}

}

void ABaseCharacter::RightB()
{
	IsRightB = true;
	if (IsRightA && !IsRightAB)
	{
		IsRightAB = true;
		RedDot->SetActorHiddenInGame(false);
	}
}

void ABaseCharacter::RightBEnd()
{
	IsRightB = false;
	if (IsRightAB)
	{
		IsRightAB = false;
		RightABTimer = 0;
		RedDot->SetActorHiddenInGame(true);
		FVector StartVec = RightAim->GetComponentLocation();
		FVector EndVec = StartVec + RightAim->GetForwardVector() * 100;
		//FRotator Rot = (EndVec - StartVec).Rotation();
		FRotator Rot = RightAim->GetComponentRotation();
		ServerSpawnThrowingWeapon(EndVec, Rot);
		ServerResetColorChange(FString("Right"));
	}

}

void ABaseCharacter::RightA()
{
	IsRightA = true;
	if (IsRightB && !IsRightAB)
	{
		IsRightAB = true;
		RedDot->SetActorHiddenInGame(false);
	}
}

void ABaseCharacter::RightAEnd()
{
	IsRightA = false;
	if (IsRightAB)
	{
		IsRightAB = false;
		RightABTimer = 0;
		RedDot->SetActorHiddenInGame(true);

		FVector StartVec = RightAim->GetComponentLocation();
		FVector EndVec = StartVec + RightAim->GetForwardVector() * 10;
		//FRotator Rot = (EndVec - StartVec).Rotation();
		FRotator Rot = RightAim->GetComponentRotation();
		ServerSpawnThrowingWeapon(EndVec, Rot);
		ServerResetColorChange(FString("Right"));
	}
}

void ABaseCharacter::OnLeftHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GrabbedActorLeft)
	{
		GrabbedActorLeft = Cast<ABaseGrabbableActor>(OtherActor);
	}

}

void ABaseCharacter::OnLeftHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GrabbedActorLeft && GrabbedActorLeft == Cast<ABaseGrabbableActor>(OtherActor))
	{
		GrabbedActorLeft = NULL;
	}

}

void ABaseCharacter::OnRightHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GrabbedActorRight)
	{
		GrabbedActorRight = Cast<ABaseGrabbableActor>(OtherActor);
	}

}

void ABaseCharacter::OnRightHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GrabbedActorRight && GrabbedActorRight == Cast<ABaseGrabbableActor>(OtherActor))
	{
		GrabbedActorRight = NULL;
	}

}

void ABaseCharacter::ServerGrabTheActor_Implementation(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition)
{
	//GrabbedActor->MeshComp->SetSimulatePhysics(false);
	//if (GrabPosition == FString("Left"))
	//{
	//	IsLeftGrab = true;
	//	GrabbedActor->AttachToComponent(LeftHand, FAttachmentTransformRules::KeepWorldTransform);
	//}
	//else if (GrabPosition == FString("Right"))
	//{
	//	IsRightGrab = true;
	//	GrabbedActor->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);
	//}
	MulticastGrabTheActor(GrabbedActor, GrabPosition);

}
void ABaseCharacter::MulticastGrabTheActor_Implementation(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition)
{
	GrabbedActor->MeshComp->SetSimulatePhysics(false);
	if (GrabPosition == FString("Left"))
	{
		IsLeftGrab = true;
		GrabbedActor->AttachToComponent(LeftHand, FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (GrabPosition == FString("Right"))
	{
		IsRightGrab = true;
		GrabbedActor->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);
	}

}

void ABaseCharacter::ServerUnGrabTheActor_Implementation(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition, FVector RightDirThrow, FQuat RightRotThrow)
{

	MulticastUnGrabTheActor(GrabbedActor, GrabPosition, RightDirThrow, RightRotThrow);

}

void ABaseCharacter::MulticastUnGrabTheActor_Implementation(ABaseGrabbableActor* GrabbedActor, const FString& GrabPosition, FVector RightDirThrow, FQuat RightRotThrow)
{
	GrabbedActor->MeshComp->SetSimulatePhysics(true);
	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GrabbedActor->IsThrow = true;
	if (GrabPosition == FString("Left"))
	{
		IsLeftGrab = false;
		GrabbedActorLeft = NULL;
	}
	else if (GrabPosition == FString("Right"))
	{
		IsRightGrab = false;
		GrabbedActor->MeshComp->AddForce(ThrowPower * RightDirThrow);
	
		float Angle;
		FVector Axis;
		float dt = GetWorld()->DeltaTimeSeconds;
		RightRotThrow.ToAxisAndAngle(Axis, Angle);
		FVector AngularVelocity = (1.0f / dt) * Angle * Axis;
		GrabbedActor->MeshComp->SetPhysicsAngularVelocityInRadians(AngularVelocity * ToquePower, true);
	
		GrabbedActorRight = NULL;
	}

}

void ABaseCharacter::SetGrabInfo()
{
	if (IsRightGrab)
	{
		RightThrowDir = RightHand->GetComponentLocation() - RightPrevLoc;
		RightThrowRot = RightHand->GetComponentQuat() * RightPrevRot.Inverse();
	
		RightPrevLoc = RightHand->GetComponentLocation();
		RightPrevRot = RightHand->GetComponentQuat();
	}
}



FVector ABaseCharacter::BlackHoleTrace()
{
	LeftXTraces.RemoveAt(0, LeftXTraces.Num());

	FVector CurPos = LeftHand->GetComponentLocation();
	FVector Dir = LeftHand->GetRightVector() * BlackHoleForwardPower;
	
	LeftXTraces.Add(CurPos);
	
	bool IsHit;
	for (int32 i = 0; i < 40; i++)
	{
		FVector StartPos = CurPos;
		Dir += FVector::UpVector * Gravity * UnitTime;
		CurPos += Dir * UnitTime;

		FHitResult HitInfo;
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(this);
		Param.AddIgnoredActor(Blackhole);

		IsHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, CurPos, ECC_Visibility, Param);
		
		if (IsHit)
		{
			LeftXTraces.Add(HitInfo.Location);
			break;
		}
	}
	if (IsHit)
	{
		return LeftXTraces[LeftXTraces.Num() - 1];
	}
	else
	{
		return FVector(0, 0, -10000);
	}
	
}

void ABaseCharacter::ColorChange(float Rate, FString Position)
{
	if (Position == FString("Left"))
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(1, 1, 1), FVector(0, 1, 0), Rate);
		LeftHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)ColorVector);
	}
	else if (Position == FString("Right"))
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(0, 0, 0), FVector(1, 0, 0), Rate);
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)ColorVector);
	}
}

void ABaseCharacter::ServerColorChange_Implementation(float Rate, const FString& Position)
{
	MulticastColorChange(Rate, Position);
}
void ABaseCharacter::MulticastColorChange_Implementation(float Rate, const FString& Position)
{
	if (Position == FString("LeftY"))
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(1, 1, 1), FVector(0, 1, 0), Rate);
		LeftHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)ColorVector);
	}
	else if (Position == FString("LeftX"))
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(1, 1, 1), FVector(0, 0, 1), Rate);
		LeftHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)ColorVector);
	}
	else if (Position == FString("Right"))
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(0, 0, 0), FVector(1, 0, 0), Rate);
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)ColorVector);
	}
}

void ABaseCharacter::ResetColorChange(FString Position)
{
	if (Position == FString("Left"))
	{
		LeftHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(1, 1, 1));
	}
	else if (Position == FString("Right"))
	{
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(0, 0, 0));
	}
}

void ABaseCharacter::ServerResetColorChange_Implementation(const FString& Position)
{
	MulticastResetColorChange(Position);
}


void ABaseCharacter::MulticastResetColorChange_Implementation(const FString& Position)
{
	if (Position == FString("Left"))
	{
		LeftHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(1, 1, 1));
	}
	else if (Position == FString("Right"))
	{
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(0, 0, 0));
	}
}
void ABaseCharacter::ServerSpawnGrabbableActor_Implementation()
{
	ABaseGrabbableActor* GrabActor = GetWorld()->SpawnActor<ABaseGrabbableActor>(SpawnGrabbedActor, LeftHand->GetComponentLocation(), LeftHand->GetComponentRotation());
}

void ABaseCharacter::ServerVRSetting_Implementation()
{
	Blackhole = GetWorld()->SpawnActor<ABlackhole>(SpawnBlackhole, FVector(0, 0, -100), FRotator(0, 0, 0));
	MulticastVRSetting();
}

void ABaseCharacter::MulticastVRSetting_Implementation()
{
	GetMesh()->SetVisibility(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	camComp->SetActive(false);
	VRCamera->SetActive(true);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("VRPlayerPreset"));

	HeadMesh->SetVisibility(true);
	HeadComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LeftHand->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightHand->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LeftHandMesh->SetVisibility(true);
	LeftHandMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	RightHandMesh->SetVisibility(true);
	RightHandMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LeftGrip->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightAim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftAim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftHandBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightHandBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


}

void ABaseCharacter::ServerPCSetting_Implementation()
{
	MulticastPCSetting();
}

void ABaseCharacter::MulticastPCSetting_Implementation()
{
	HeadMesh->SetVisibility(false);
	HeadComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftHandMesh->SetVisibility(false);
	LeftHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightHandMesh->SetVisibility(false);
	RightHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftGrip->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightAim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftAim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	camComp->SetActive(true);
	VRCamera->SetActive(false);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerPreset"));

	GetMesh()->SetVisibility(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseCharacter::ServerBlackholeSet_Implementation(float Rate, FVector Loc)
{
	MulticastBlackholeSet(Rate, Loc);
}

void ABaseCharacter::MulticastBlackholeSet_Implementation(float Rate, FVector Loc)
{
	Blackhole->SetActorScale3D(UKismetMathLibrary::VLerp(Blackhole->OriginSize, Blackhole->LimitSize, Rate));
	if (Loc != FVector(0, 0, -10000))
	{
		Blackhole->SetActorLocation(Loc);
	}
}

void ABaseCharacter::ServerBlackholeReset_Implementation()
{
	MulticastBlackholeReset();
}

void ABaseCharacter::MulticastBlackholeReset_Implementation()
{
	Blackhole->SetActorScale3D(Blackhole->OriginSize);
	Blackhole->SetActorLocation(FVector(0, 0, -10000));
}

void ABaseCharacter::ServerVRTransform_Implementation(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform)
{
	//MulticastHandTransform(LeftTransform, RightTransform);
	VRCamera->SetRelativeTransform(HeadTransform);
	LeftHand->SetRelativeTransform(LeftTransform);
	RightHand->SetRelativeTransform(RightTransform);
}

void ABaseCharacter::MulticastVRTransform_Implementation(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform)
{
	//LeftHand->SetRelativeTransform(LeftTransform);
	//RightHand->SetRelativeTransform(RightTransform);
}

void ABaseCharacter::ServerBlackholeActivate_Implementation(bool IsActive)
{
	IsBlackholeSet = IsActive;
}

void ABaseCharacter::ServerSpawnThrowingWeapon_Implementation(FVector SpawnLoc, FRotator SpawnRot)
{
	AThrowingWeapon* ThrowingWeaponActor = GetWorld()->SpawnActor<AThrowingWeapon>(SpawnThrowingWeapon, SpawnLoc, SpawnRot);
}

void ABaseCharacter::SetRedDot()
{
	FVector DotStart = RightAim->GetComponentLocation();
	FVector DotEnd = DotStart + RightAim->GetForwardVector() * 100000;

	FHitResult HitInfo;
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	float Distance;
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitInfo, DotStart, DotEnd, ECC_Visibility, Param);
	if (IsHit)
	{
		RedDot->SetActorLocation(HitInfo.Location);
		Distance = HitInfo.Distance;
	}
	else 
	{
		RedDot->SetActorLocation(DotEnd);
		Distance = (DotEnd - DotStart).Size();
	}
	RedDot->SetActorScale3D(FVector(Distance / 500));
	RedDot->SetActorRotation((-RedDot->GetActorLocation() + VRCamera->GetComponentLocation()).Rotation());
}

void ABaseCharacter::VRGetDamage(float Damage)
{
	VRCurHP -= Damage;
	ServerVRGetDamage(Damage, 1 - VRCurHP/VRMaxHP);
}

void ABaseCharacter::ServerVRGetDamage_Implementation(float Damage, float Rate)
{
	MulticastVRGetDamage(Damage, Rate);
}

void ABaseCharacter::MulticastVRGetDamage_Implementation(float Damage, float Rate)
{
	if (Rate <= 0.45)
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(0, 0.8, 0), FVector(0.8, 0.8, 0), Rate * 20 / 9);
		HeadMat->SetVectorParameterValue(FName("HeadColor"), (FLinearColor)ColorVector);
	}
	else if (Rate <= 0.9)
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(0.8, 0.8, 0), FVector(0.8, 0, 0), (Rate - 0.45) * 20 / 9);
		HeadMat->SetVectorParameterValue(FName("HeadColor"), (FLinearColor)ColorVector);
	}
}