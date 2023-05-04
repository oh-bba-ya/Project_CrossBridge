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
#include "Components/CapsuleComponent.h"
#include "Objects/BaseGrabbableActor.h"
#include "Skill/Freeze.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects/Blackhole.h"
#include "Objects/ThrowingWeapon.h"
#include "VRCharacter/Widget/VRStatusWidget.h"
#include "Math/Vector.h"
#include "Weapon/Cannon.h"
#include "NiagaraComponent.h"
#include "Objects/Thunder.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "PickupItem/HomingItem.h"
#include "Objects/TrashSpawningPool.h"


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

	RightGrip = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightGrip"));
	RightGrip->SetupAttachment(RootComponent);
	RightGrip->SetTrackingMotionSource(FName("RightGrip"));

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

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(RightHand);
	SwordMesh->SetRelativeLocation(FVector(5.1, -4, -4.8));
	SwordMesh->SetRelativeRotation(FRotator(-75, 0, 0));
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordMesh->SetVisibility(false);

	InvisibleSwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvisibleSwordMesh"));
	InvisibleSwordMesh->SetupAttachment(RightHand);
	InvisibleSwordMesh->SetRelativeLocation(FVector(5.1, -4, -4.8));
	InvisibleSwordMesh->SetRelativeRotation(FRotator(-75, 0, 0));
	InvisibleSwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InvisibleSwordMesh->SetVisibility(false);

	SwordComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordComp"));
	SwordComp->SetupAttachment(SwordMesh);
	SwordComp->SetCollisionProfileName(TEXT("NoCollision"));
	SwordComp->SetRelativeLocation(FVector(3, 1, 46));
	SwordComp->SetBoxExtent(FVector(3, 3, 38));

	GrabbableObjectCreateEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GrabbableObjectCreateEffect"));
	GrabbableObjectCreateEffect->SetupAttachment(LeftHand);
	GrabbableObjectCreateEffect->SetRelativeLocation(FVector(3, 4, -6));
	GrabbableObjectCreateEffect->SetVisibility(false);

	BlackholeTraceComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BlackholeTraceComp"));
	BlackholeTraceComp->SetupAttachment(RootComponent);

	VRStatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("VRStatus"));
	VRStatusWidget->SetupAttachment(LeftHand);
	VRStatusWidget->SetRelativeLocation(FVector(-2, 0.15, 6));
	VRStatusWidget->SetRelativeRotation(FRotator(8, 92, -70));
	VRStatusWidget->SetRelativeScale3D(FVector(0.025));
	VRStatusWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> VRHeadMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/VR_Head.VR_Head'"));
	if (VRHeadMesh.Succeeded())
	{
		HeadMesh->SetSkeletalMesh(VRHeadMesh.Object);
		HeadMesh->SetRelativeLocation(FVector(0, 0, -170));
		HeadMesh->SetRelativeRotation(FRotator(0, -90, 0));
		HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> LeftMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (LeftMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(LeftMesh.Object);
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
	}

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> RightMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (RightMesh.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(RightMesh.Object);
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> HandAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/MannequinsXR/Meshes/ABP_MannequinsXR.ABP_MannequinsXR_C'"));
	if (HandAnim.Succeeded())
	{
		LeftHandMesh->SetAnimInstanceClass(HandAnim.Class);
		RightHandMesh->SetAnimInstanceClass(HandAnim.Class);
	}

	ConstructorHelpers::FClassFinder<UVRStatusWidget> VRStat(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/VRPlayer/Widget/BP_VRStatusWidget.BP_VRStatusWidget_C'"));
	if (VRStat.Succeeded())
	{

		VRStatusWidget->SetWidgetClass(VRStat.Class);
	}

	LeftHandBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnLeftHandOverlap);
	LeftHandBox->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnLeftHandEndOverlap);
	RightHandBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnRightHandOverlap);
	RightHandBox->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnRightHandEndOverlap);
	SwordComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnSwordOverlap);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Health 초기화.
	if (HasAuthority())
	{
		SetCurrentHealth(MaxHP);
	}

	UE_LOG(LogTemp,Warning,TEXT("Current HP : %.1f"),CurrentHP);

	if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BaseContext, 0);
			Subsystem->AddMappingContext(IMC_VRInput, 0);
			Subsystem->AddMappingContext(IMC_VRHand, 0);
		}
	}

	// 내가 조종하는 캐릭터
	if (GetController() != nullptr && GetController()->IsLocalController())
	{
		// Player Name
		GameInstance = GetGameInstance();

		if (GameInstance != nullptr)
		{
			MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

			if (MultiplayerSessionsSubsystem)
			{
				ServerSetName(MultiplayerSessionsSubsystem->SessionID.ToString());
			}
		}
	}

	if (OverheadWidget)
	{
		overhead = Cast<UOverheadWidget>(OverheadWidget->GetWidget());
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IsVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	if (IsVR)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);

		ServerVRSetting();

		RedDot = GetWorld()->SpawnActor<AActor>(SpawnRedDot, GetActorLocation(), GetActorRotation());
		RedDot->SetActorHiddenInGame(true);
		RedDot->SetActorEnableCollision(false);
		VRController = UGameplayStatics::GetPlayerController(this, 0);
	}
	else
	{
		ServerPCSetting();
	}

	VRStatus = Cast<UVRStatusWidget>(VRStatusWidget->GetWidget());
	VRStatus->SetHPBar(0);
	UMaterialInterface *HeadBase = HeadMesh->GetMaterial(0);
	if (HeadBase)
	{
		HeadMat = HeadMesh->CreateDynamicMaterialInstance(0, HeadBase);
	}
	UMaterialInterface *LeftHandBase = LeftHandMesh->GetMaterial(0);
	if (LeftHandBase)
	{
		LeftHandMat = LeftHandMesh->CreateDynamicMaterialInstance(0, LeftHandBase);
	}
	UMaterialInterface *RightHandBase = RightHandMesh->GetMaterial(0);
	if (RightHandBase)
	{
		RightHandMat = RightHandMesh->CreateDynamicMaterialInstance(0, RightHandBase);
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(0, 0, 0));
	}
	UMaterialInterface *SwordBase = SwordMesh->GetMaterial(3);
	if (SwordBase)
	{
		SwordMat = SwordMesh->CreateDynamicMaterialInstance(3, SwordBase);
		SwordMat->SetScalarParameterValue(FName("SwordOpacity"), 0);
	}
	//UMaterialInterface *InvisibleSwordBase = InvisibleSwordMesh->GetMaterial(3);
	//if (InvisibleSwordBase)
	//{
	//	InvisibleSwordMat = InvisibleSwordMesh->CreateDynamicMaterialInstance(3, InvisibleSwordBase);
	//	InvisibleSwordMat->SetScalarParameterValue(FName("SwordOpacity"), 0);
	//}

	ServerResetColorChange(FString("SwordDissolve"));

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentHP <= 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("Die"));
	}
	
	if (bJetPackActive)
	{
		AddMovementInput(FVector(0, 0, JetPackSpeed));
	}

	if (overhead)
	{
		overhead->DisplayText->SetText(FText::FromString(myName));
	}

	if (freeze != nullptr)
	{
		freeze->SetActorLocation(GetActorLocation());
	}

	if (IsVR)
	{
		ServerVRTransform(VRCamera->GetRelativeTransform(), LeftHand->GetRelativeTransform(), RightHand->GetRelativeTransform());

		float AimDotProduct = FVector::DotProduct(RightAim->GetForwardVector(), LeftAim->GetForwardVector());
		float GripDotProduct = FVector::DotProduct(RightGrip->GetRightVector(), LeftGrip->GetRightVector());

		if (AimDotProduct >= 0.9 && GripDotProduct >= 0.9)
		{
			VRHealTime += DeltaTime;
			if (VRCurHP < VRMaxHP)
			{
				if (VRHealTime <= VRHealDelayTime)
				{
					ServerColorChange(VRHealTime / VRHealDelayTime, FString("Heal"));
				}
				else
				{
					if (!IsHeal)
					{
						IsHeal = true;
						VRController->PlayHapticEffect(HealHaptic, EControllerHand::Right, 1, true);
						VRController->PlayHapticEffect(HealHaptic, EControllerHand::Left, 1, true);
					}
					VRGetDamage(-0.5 * DeltaTime);
				}
			}
			else
			{
				if (IsHeal)
				{
					IsHeal = false;
					VRController->StopHapticEffect(EControllerHand::Right);
					VRController->StopHapticEffect(EControllerHand::Left);
					VRHealTime = 0;
					ServerResetColorChange("Right");
				}
			}
		}
		else if ((AimDotProduct < 0.9 || GripDotProduct < 0.9) && VRHealTime > 0)
		{
			VRHealTime = 0;
			if (IsHeal)
			{
				IsHeal = false;
				VRController->StopHapticEffect(EControllerHand::Right);
				VRController->StopHapticEffect(EControllerHand::Left);
				ServerResetColorChange("Right");
			}
		}

		float StatusWidgetDotProduct = FVector::DotProduct(LeftHand->GetRightVector(), -VRCamera->GetForwardVector());
		if (StatusWidgetDotProduct >= 0.8)
		{
			if (!VRStatusWidget->IsVisible())
			{
				VRStatusWidget->SetVisibility(true);
			}
		}
		else
		{
			if (VRStatusWidget->IsVisible())
			{
				VRStatusWidget->SetVisibility(false);
			}
		}
	}

	SetGrabInfo();

	if (IsLeftY)
	{
		LeftYTimer += DeltaTime;
		if (LeftYTimer / LeftYCastTime <= 1)
		{

			// ColorChange(LeftYTimer / 5, FString("Left"));
			ServerColorChange(LeftYTimer / LeftYCastTime, FString("LeftY"));
		}
	}

	if (IsLeftX && !IsBlackholeCool)
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
			// Blackhole->ServerBlackholeSize(LeftXTimer / LeftXCastTime);
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
			BlackholeTraceComp->SetVisibility(false);
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
		IsBlackholeCool = true;
	}

	if (IsBlackholeCool)
	{
		if (BlackholeCoolTime == 0)
		{
			VRStatus->SetImageColor(FString("Blackhole"), true);
		}
		BlackholeCoolTime += DeltaTime;
		VRStatus->SetCooltimeText(FString("Blackhole"), BlackholeCoolTimeLimit - BlackholeCoolTime);
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%f"), BlackholeCoolTime));
		if (BlackholeCoolTime > BlackholeCoolTimeLimit)
		{
			VRStatus->SetCooltimeText(FString("Blackhole"), 0);
			VRStatus->SetImageColor(FString("Blackhole"), false);
			IsBlackholeCool = false;
			BlackholeCoolTime = 0;
		}
	}

	if (IsRedDotSet)
	{
		RightBTimer += DeltaTime;
		SetRedDot();
		if (RightBTimer / RightBCastTime <= 1)
		{
			ServerColorChange(RightBTimer / RightBCastTime, FString("Right"));
		}
	}

	if (IsRightA && !IsSwordCool && !IsSwordSet)
	{

		float AimDotProduct = FVector::DotProduct(RightAim->GetForwardVector(), LeftAim->GetForwardVector());
		float GripDotProduct = FVector::DotProduct(RightGrip->GetRightVector(), -LeftGrip->GetRightVector());
		if (AimDotProduct >= 0.9 && GripDotProduct >= 0.9)
		{
			if (SwordSetTime == 0)
			{
				VRController->PlayHapticEffect(SwordCastHaptic, EControllerHand::Right, 1, true);
				VRController->PlayHapticEffect(SwordCastHaptic, EControllerHand::Left, 1, true);
			}
			SwordSetTime += DeltaTime;
			if (SwordSetTime / SwordCompleteTime <= 1)
			{
				ServerColorChange(SwordSetTime / SwordCompleteTime, FString("SwordDissolve"));
				ServerColorChange(SwordSetTime / SwordCompleteTime, FString("RightA"));
			}
			else
			{
				VRController->StopHapticEffect(EControllerHand::Right);
				VRController->StopHapticEffect(EControllerHand::Left);

				VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
				VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left);

				IsSwordSet = true;
			}
		}
		else
		{
			if (!IsSwordSet && SwordSetTime != 0)
			{
				VRController->StopHapticEffect(EControllerHand::Right);
				VRController->StopHapticEffect(EControllerHand::Left);
				SwordSetTime = 0;
				//ServerResetColorChange(FString("SwordOpacity"));
				ServerResetColorChange(FString("SwordDissolve"));
				ServerResetColorChange(FString("Left"));
			}
		}
	}

	if (IsSwordSet)
	{
		if (SwordActivateTime == 0)
		{
			SwordComp->SetCollisionProfileName(TEXT("VRPlayerSwordPreset"));
			ServerResetColorChange(FString("SwordOpacity"));
		}
		SwordActivateTime += DeltaTime;
	
		if (SwordActivateTime > SwordActivateLimitTime)
		{
			IsSwordSet = false;
			IsSwordCool = true;
			SwordSetTime = 0;
			SwordActivateTime = 0;
			SwordComp->SetCollisionProfileName(TEXT("NoCollision"));
			ServerResetColorChange(FString("SwordDissolve"));
			ServerResetColorChange(FString("Left"));
		}

	}
	
	if (IsSwordDamageCool)
	{
		SwordDamageCoolTime += DeltaTime;
		if (SwordDamageCoolTime > SwordDamageCoolTimeLimit)
		{
			IsSwordDamageCool = false;
			SwordDamageCoolTime = 0;
		}
	}

	if (IsSwordCool)
	{
		if (SwordCoolTime == 0)
		{
			VRStatus->SetImageColor(FString("Sword"), true);
		}
		SwordCoolTime += DeltaTime;
		VRStatus->SetCooltimeText(FString("Sword"), SwordCoolTimeLimit - SwordCoolTime);
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%f"), SwordCoolTime));
		if (SwordCoolTime > SwordCoolTimeLimit)
		{
			VRStatus->SetCooltimeText(FString("Sword"), 0);
			VRStatus->SetImageColor(FString("Sword"), false);
			IsSwordCool = false;
			SwordCoolTime = 0;
		}
	}

	// FVector StartPos = LeftGrip->GetComponentLocation() + LeftAim->GetForwardVector() * 10;
	// FVector EndPos = StartPos + LeftGrip->GetRightVector() * 1000;
	//FVector StartPos = LeftHand->GetComponentLocation();
	//FVector EndPos = StartPos + LeftHand->GetUpVector() * 1000;
	//DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, -1, 0, 1);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		EnhancedInputComponent->BindAction(InputMovementAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		EnhancedInputComponent->BindAction(InputJumpAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Jump);
		EnhancedInputComponent->BindAction(InputJumpAction, ETriggerEvent::Completed, this, &ABaseCharacter::Release_Jump);
		EnhancedInputComponent->BindAction(InputAttackAction, ETriggerEvent::Started, this, &ABaseCharacter::Attack);
		EnhancedInputComponent->BindAction(InputContextualAction, ETriggerEvent::Started, this, &ABaseCharacter::ContextualActionPressed);
		EnhancedInputComponent->BindAction(InputContextualAction, ETriggerEvent::Completed, this, &ABaseCharacter::ContextualActionReleased);
		EnhancedInputComponent->BindAction(InputDropWeaponAction, ETriggerEvent::Started, this, &ABaseCharacter::DropWeapon);
		EnhancedInputComponent->BindAction(InputRollingAction, ETriggerEvent::Started, this, &ABaseCharacter::RollingActionPressed);
		EnhancedInputComponent->BindAction(InputSlidingAction, ETriggerEvent::Started, this, &ABaseCharacter::SlidingActionPressed);
		EnhancedInputComponent->BindAction(InputPickupAction, ETriggerEvent::Started, this, &ABaseCharacter::CanonFire);

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

void ABaseCharacter::Move(const FInputActionValue &Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ABaseCharacter::Look(const FInputActionValue &Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ABaseCharacter::Jump()
{
	if (Fuel > 0)
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
	UE_LOG(LogTemp, Warning, TEXT("Base Attack"));
	Fire();
}

void ABaseCharacter::ContextualActionPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Base ContextualAction Pressed"));
	FreezeSpawn();
}

void ABaseCharacter::ContextualActionReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Base ContextualAction Released"));
	RemoveFreeze();
}

/** Sliding , Rolling Functions()*/
#pragma region Sliding, Rolling Function()
void ABaseCharacter::RollingActionPressed()
{
	if(freeze == nullptr)
	{
		Server_RollingActionPressed();
	}
}

void ABaseCharacter::RollingActionReleased()
{
	Server_RollingActionReleased();

}

void ABaseCharacter::Server_RollingActionPressed_Implementation()
{
	Multicast_RollingActionPressed();
}

void ABaseCharacter::Server_RollingActionReleased_Implementation()
{
	Multicast_RollingActionReleased();
}

void ABaseCharacter::Multicast_RollingActionPressed_Implementation()
{

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	if (RollingMontage)
	{
		PlayAnimMontage(RollingMontage);
	}
	GetCapsuleComponent()->SetCapsuleHalfHeight(20.f);

}

void ABaseCharacter::Multicast_RollingActionReleased_Implementation()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}



void ABaseCharacter::SlidingActionPressed()
{
	if(freeze == nullptr)
	{
		Server_SlidingActionPressed();
	}
}

void ABaseCharacter::SlidingActionRelease()
{
	Server_SlidingActionReleased();
}

void ABaseCharacter::Server_SlidingActionPressed_Implementation()
{
	Multicast_SlidingActionPressed();
}

void ABaseCharacter::Server_SlidingActionReleased_Implementation()
{
	Multicast_SlidingActionReleased();
}



void ABaseCharacter::Multicast_SlidingActionPressed_Implementation()
{
	Crouch();
	//GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	if (SlidingMontage)
	{
		PlayAnimMontage(SlidingMontage);
	}
	GetCapsuleComponent()->SetCapsuleHalfHeight(20.f);
}

void ABaseCharacter::Multicast_SlidingActionReleased_Implementation()
{
	UnCrouch();
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
#pragma endregion

/** JetPack */
#pragma region JetPack()

void ABaseCharacter::Release_Jump()
{
	DeActivateJetPack();
}

void ABaseCharacter::FillUpFuel()
{
	if (Fuel >= MaxFuel)
	{
		GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
		Fuel = MaxFuel;
	}
	Fuel += FuelConsumptionSpeed;
}

void ABaseCharacter::FuelConsumption(float value)
{
	Fuel = FMath::Clamp(Fuel - value, 0, MaxFuel);
}

void ABaseCharacter::ActivateJetPack()
{
	Server_ActivateJetPack();
}

void ABaseCharacter::Server_ActivateJetPack_Implementation()
{
	if (freeze == nullptr)
	{
		bJetPackActive = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->AirControl = 1.f;

		FuelConsumption(FuelConsumptionSpeed);
		GetWorld()->GetTimerManager().ClearTimer(fuelTimer);
	}
}

void ABaseCharacter::DeActivateJetPack()
{
	Server_DeActivateJetPack();
}

void ABaseCharacter::Server_DeActivateJetPack_Implementation()
{
	if (freeze == nullptr)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetWorld()->GetTimerManager().SetTimer(fuelTimer, this, &ABaseCharacter::FillUpFuel, FuelRechargeSpeed, true, FuelRechargeDelay);
	}
	bJetPackActive = false;
	GetCharacterMovement()->AirControl = 0.2f;
}

#pragma endregion

/** Health() */
#pragma region Health()
void ABaseCharacter::Server_TakeDamage_Implementation(float value)
{
	if (CurrentHP > 0)
	{
		SubTractHealth(value);
	}
}

void ABaseCharacter::Server_RecoveryHP_Implementation(float value)
{
	if (CurrentHP < 100)
	{
		PlusHealth(value);
	}
}

void ABaseCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHP = FMath::Clamp(healthValue, 0.f, MaxHP);
	}
}

void ABaseCharacter::PlusHealth(int32 value)
{
	CurrentHP = FMath::Clamp(CurrentHP + value, 0.f, MaxHP);
}

void ABaseCharacter::SubTractHealth(int32 value)
{
	CurrentHP = FMath::Clamp(CurrentHP - value, 0.f, MaxHP);
}

#pragma endregion

/** Fire */
#pragma region Fire()

void ABaseCharacter::Fire()
{
	if (myWeapon != nullptr && freeze == nullptr)
	{
		if(myWeapon->GetbFireDelay())
		{
			Server_Fire();
			myWeapon->Fire(this);
		}
	}
}

void ABaseCharacter::Server_Fire_Implementation()
{
	Multicast_Fire();
}

void ABaseCharacter::Multicast_Fire_Implementation()
{
	if (fireMontage != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire Montage"));
		PlayAnimMontage(fireMontage);
	}
}
#pragma endregion

/** Weapon */
void ABaseCharacter::DropWeapon()
{
	if (GetController() != nullptr && GetController()->IsLocalController() && myWeapon != nullptr)
	{
		myWeapon->DropWeapon(this);
	}

	if (GetController() != nullptr && GetController()->IsLocalController() && myHoming != nullptr)
	{
		myHoming->DropItem(this);
	}
}

/** Player Name */
void ABaseCharacter::ServerSetName_Implementation(const FString &name)
{
	myName = name;

	ACrossPlayerState *ps = Cast<ACrossPlayerState>(GetPlayerState());

	if (ps != nullptr)
	{
		ps->SetPlayerName(name);
	}
}

/** Freeze Skill */
#pragma region Skill Freeze
void ABaseCharacter::FreezeSpawn()
{
	Server_FreezeSpawn();
}

void ABaseCharacter::Server_FreezeSpawn_Implementation()
{
	freeze = GetWorld()->SpawnActor<AFreeze>(FreezeFactory, GetActorLocation(), GetActorRotation());
	if (freeze != nullptr)
	{
		freeze->SetOwner(this);
	}

	GetCharacterMovement()->DisableMovement();
}

void ABaseCharacter::RemoveFreeze()
{
	Server_RemoveFreeze();
}

void ABaseCharacter::Server_RemoveFreeze_Implementation()
{
	freeze->Destroy();
	if (freeze != nullptr)
	{
		freeze = nullptr;
	}

	DeActivateJetPack();
}
#pragma endregion

#pragma region Canon
void ABaseCharacter::CanonFire()
{
	if(mycanon != nullptr)
	{
		if(myHoming != nullptr)
		{
			mycanon->Reload(myHoming);
			
			myHoming->UsingItem(this);
			if(myHoming != nullptr)
			{
				myHoming = nullptr;
			}
		}
		else
		{
			if(mycanon->GetFireDelay())
			{
				mycanon->HomingFire(this);
			}
		}
	}


}

#pragma endregion

// 서버에 복제 등록하기 위한 함수
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bJetPackActive);
	DOREPLIFETIME(ABaseCharacter, Fuel);
	DOREPLIFETIME(ABaseCharacter, CurrentHP);
	DOREPLIFETIME(ABaseCharacter, myName);
	DOREPLIFETIME(ABaseCharacter, freeze);
	DOREPLIFETIME(ABaseCharacter, IsBlackholeSet);
	DOREPLIFETIME(ABaseCharacter, VRCurHP);
	DOREPLIFETIME(ABaseCharacter, myHoming);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ABaseCharacter::VRMove(const FInputActionValue &Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
}

void ABaseCharacter::Turn(const FInputActionValue &Values)
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
	if (!IsLeftIndexCurl && !IsLeftGrasp && !IsLeftX)
	{
		IsLeftY = true;
	}
}
void ABaseCharacter::LeftYEnd()
{
	if (LeftYTimer / LeftYCastTime > 1)
	{
		ServerSpawnThunder();
	}
	IsLeftY = false;
	LeftYTimer = 0;
	// ResetColorChange(FString("Left"));
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
	IsLeftX = false;
	if (!IsBlackholeCool)
	{
		ServerBlackholeActivate(true);
		LeftXTimer = 0;
		ServerResetColorChange(FString("Left"));
	}
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
	if (!IsRightA && !IsRedDotSet)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("READY"));
		VRController->PlayHapticEffect(BulletCastHaptic, EControllerHand::Right);
		IsRedDotSet = true;
		RedDot->SetActorHiddenInGame(false);
	}
}

void ABaseCharacter::RightBEnd()
{
	IsRightB = false;
	if (IsRedDotSet)
	{
		if (RightBTimer / RightBCastTime > 1)
		{
			FVector StartVec = RightAim->GetComponentLocation();
			FVector EndVec = StartVec + RightAim->GetForwardVector() * 1;
			// FRotator Rot = (EndVec - StartVec).Rotation();
			FRotator Rot = RightAim->GetComponentRotation();
			ServerSpawnThrowingWeapon(EndVec, Rot);
			VRController->PlayHapticEffect(BulletFireHaptic, EControllerHand::Right);
		}
		else
		{
			VRController->StopHapticEffect(EControllerHand::Right);
		}
		IsRedDotSet = false;
		RightBTimer = 0;
		RedDot->SetActorHiddenInGame(true);
		ServerResetColorChange(FString("Right"));
	}
}

void ABaseCharacter::RightA()
{
	VRGetDamage(5);
	if (!IsRightA && !IsSwordCool)
	{
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
		ServerColorChange(0, FString("SwordOpacity"));
	}
	IsRightA = true;
}

void ABaseCharacter::RightAEnd()
{
	if (IsSwordSet)
	{
		IsSwordSet = false;
		IsSwordCool = true;
		SwordComp->SetCollisionProfileName(TEXT("NoCollision"));
	}
	if (IsSwordDamageCool) 
	{
		IsSwordDamageCool = false;
		SwordDamageCoolTime = 0;
	}
	IsRightA = false;
	SwordSetTime = 0;
	SwordActivateTime = 0;
	ServerResetColorChange(FString("SwordOpacity"));
	ServerResetColorChange(FString("SwordDissolve"));
	ServerResetColorChange(FString("Left"));
}

void ABaseCharacter::OnLeftHandOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!GrabbedActorLeft)
	{
		GrabbedActorLeft = Cast<ABaseGrabbableActor>(OtherActor);
	}
}

void ABaseCharacter::OnLeftHandEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (GrabbedActorLeft && GrabbedActorLeft == Cast<ABaseGrabbableActor>(OtherActor))
	{
		GrabbedActorLeft = NULL;
	}
}

void ABaseCharacter::OnRightHandOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!GrabbedActorRight)
	{
		GrabbedActorRight = Cast<ABaseGrabbableActor>(OtherActor);
	}
}

void ABaseCharacter::OnRightHandEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (GrabbedActorRight && GrabbedActorRight == Cast<ABaseGrabbableActor>(OtherActor))
	{
		GrabbedActorRight = NULL;
	}
}

void ABaseCharacter::OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	class ABaseCharacter* Enemy = Cast<ABaseCharacter>(OtherActor);
	if (Enemy && !IsSwordDamageCool)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("IsHit"));
		IsSwordDamageCool = true;
		//Enemy->Server_TakeDamage(10);
		ServerVRAttack(FString("Sword"), Enemy);
	}
}
void ABaseCharacter::ServerGrabTheActor_Implementation(ABaseGrabbableActor *GrabbedActor, const FString &GrabPosition)
{
	// GrabbedActor->MeshComp->SetSimulatePhysics(false);
	// if (GrabPosition == FString("Left"))
	//{
	//	IsLeftGrab = true;
	//	GrabbedActor->AttachToComponent(LeftHand, FAttachmentTransformRules::KeepWorldTransform);
	// }
	// else if (GrabPosition == FString("Right"))
	//{
	//	IsRightGrab = true;
	//	GrabbedActor->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);
	// }
	MulticastGrabTheActor(GrabbedActor, GrabPosition);
}

void ABaseCharacter::MulticastGrabTheActor_Implementation(ABaseGrabbableActor *GrabbedActor, const FString &GrabPosition)
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

void ABaseCharacter::ServerUnGrabTheActor_Implementation(ABaseGrabbableActor *GrabbedActor, const FString &GrabPosition, FVector RightDirThrow, FQuat RightRotThrow)
{
	MulticastUnGrabTheActor(GrabbedActor, GrabPosition, RightDirThrow, RightRotThrow);
}

void ABaseCharacter::MulticastUnGrabTheActor_Implementation(ABaseGrabbableActor *GrabbedActor, const FString &GrabPosition, FVector RightDirThrow, FQuat RightRotThrow)
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

	FVector CurPos = LeftHand->GetComponentLocation() + LeftHand->GetUpVector()* -10 + LeftHand->GetForwardVector() * 5;
	FVector Dir = (LeftHandMesh->GetRightVector() - LeftHandMesh->GetUpVector() + LeftHandMesh->GetForwardVector()).GetSafeNormal() * BlackHoleForwardPower;

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
	BlackholeTraceComp->SetVisibility(true);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(BlackholeTraceComp, FName(TEXT("User.PointArray")), LeftXTraces);
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

void ABaseCharacter::ServerColorChange_Implementation(float Rate, const FString &Position)
{
	MulticastColorChange(Rate, Position);
}
void ABaseCharacter::MulticastColorChange_Implementation(float Rate, const FString &Position)
{
	if (Position == FString("LeftY"))
	{
		if (!GrabbableObjectCreateEffect->IsVisible())
		{
			GrabbableObjectCreateEffect->SetVisibility(true);
		}
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
	else if (Position == FString("Heal"))
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(0, 0, 0), FVector(1, 1, 1), Rate);
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)ColorVector);
	}
	else if (Position == FString("SwordOpacity"))
	{
		//float OpacityRate = UKismetMathLibrary::Lerp(0.3, 1, Rate);
		//InvisibleSwordMat->SetScalarParameterValue(FName("SwordOpacity"), OpacityRate);
		InvisibleSwordMesh->SetVisibility(true);
	}
	else if (Position == FString("SwordDissolve"))
	{
		if (!SwordMesh->IsVisible())
		{
			SwordMesh->SetVisibility(true);
		}
		float DissolveRate = UKismetMathLibrary::Lerp(0, 0.5f, Rate);
		SwordMat->SetScalarParameterValue(FName("Dissolve"), DissolveRate);
	}
	else if (Position == FString("RightA"))
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(1, 1, 1), FVector(0, 0, 0), Rate);
		LeftHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)ColorVector);
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

void ABaseCharacter::ServerResetColorChange_Implementation(const FString &Position)
{
	MulticastResetColorChange(Position);
}

void ABaseCharacter::MulticastResetColorChange_Implementation(const FString &Position)
{
	if (Position == FString("Left"))
	{
		if (GrabbableObjectCreateEffect->IsVisible())
		{
			GrabbableObjectCreateEffect->SetVisibility(false);
		}
		LeftHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(1, 1, 1));
	}
	else if (Position == FString("Right"))
	{
		RightHandMat->SetVectorParameterValue(FName("Tint"), (FLinearColor)FVector(0, 0, 0));
	}
	else if (Position == FString("SwordOpacity"))
	{
		//InvisibleSwordMat->SetScalarParameterValue(FName("SwordOpacity"), 0);
		InvisibleSwordMesh->SetVisibility(false);
	}
	else if (Position == FString("SwordDissolve"))
	{
		SwordMat->SetScalarParameterValue(FName("Dissolve"), 0);
		SwordMesh->SetVisibility(false);
	}
}
void ABaseCharacter::ServerSpawnThunder_Implementation()
{
	//ABaseGrabbableActor *GrabActor = GetWorld()->SpawnActor<ABaseGrabbableActor>(SpawnGrabbedActor, LeftHand->GetComponentLocation(), LeftHand->GetComponentRotation());
	AThunder* ThunderItem = GetWorld()->SpawnActor<AThunder>(SpawnThunder, GrabbableObjectCreateEffect->GetComponentLocation(), GrabbableObjectCreateEffect->GetComponentRotation());
	ThunderItem->AttachToComponent(LeftHand, FAttachmentTransformRules::KeepWorldTransform);
}

void ABaseCharacter::ServerVRSetting_Implementation()
{
	VRCurHP = VRMaxHP;
	Blackhole = GetWorld()->SpawnActor<ABlackhole>(SpawnBlackhole, FVector(0, 0, -1000), FRotator(0, 0, 0));
	MulticastVRSetting();
}

void ABaseCharacter::MulticastVRSetting_Implementation()
{
	GetMesh()->SetVisibility(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	camComp->SetActive(false);
	VRCamera->SetActive(true);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("VRPlayerPreset"));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.3f));

	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;
	GetCharacterMovement()->MaxFlySpeed = 600;

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

	VRStatusWidget->SetVisibility(true);
	//InvisibleSwordMesh->SetVisibility(true);
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

	VRStatusWidget->SetVisibility(false);
	SwordMesh->SetVisibility(false);
	InvisibleSwordMesh->SetVisibility(false);

	camComp->SetActive(true);
	VRCamera->SetActive(false);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerPreset"));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.2));

	springArmComp->TargetArmLength = 100;
	GetCharacterMovement()->MaxWalkSpeed = 100;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 50;
	GetCharacterMovement()->MaxFlySpeed = 100;

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
	VRCamera->SetRelativeTransform(HeadTransform);
	// MulticastHandTransform(LeftTransform, RightTransform);
	LeftHand->SetRelativeTransform(LeftTransform);
	RightHand->SetRelativeTransform(RightTransform);
}

void ABaseCharacter::MulticastVRTransform_Implementation(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform)
{
	// LeftHand->SetRelativeTransform(LeftTransform);
	// RightHand->SetRelativeTransform(RightTransform);
}

void ABaseCharacter::ServerBlackholeActivate_Implementation(bool IsActive)
{
	IsBlackholeSet = IsActive;
	if (IsActive)
	{
		Blackhole->BlackholeActiveSetting();
	}
	else
	{
		Blackhole->BlackholeDeactivate();
	}
}

void ABaseCharacter::ServerSpawnThrowingWeapon_Implementation(FVector SpawnLoc, FRotator SpawnRot)
{
	AThrowingWeapon *ThrowingWeaponActor = GetWorld()->SpawnActor<AThrowingWeapon>(SpawnThrowingWeapon, SpawnLoc, SpawnRot);
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
	ServerVRGetDamage(Damage);
}

void ABaseCharacter::ServerVRGetDamage_Implementation(float Damage)
{
	VRCurHP -= Damage;
	float Rate = 1 - (VRCurHP / VRMaxHP);
	MulticastVRGetDamage(Rate);
}

void ABaseCharacter::MulticastVRGetDamage_Implementation(float Rate)
{

	Cast<UVRStatusWidget>(VRStatusWidget->GetWidget())->SetHPBar(Rate);

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

void ABaseCharacter::ServerVRAttack_Implementation(const FString& Position, class ABaseCharacter* Enemy)
{
	if (Position == FString("Sword"))
	{
		Enemy->Server_TakeDamage(10);
	}
}
