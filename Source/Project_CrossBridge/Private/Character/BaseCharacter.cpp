// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"

#include "BaseCharacterController.h"
#include "CrossBridgeStateBase.h"
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
#include "NiagaraFunctionLibrary.h"
#include "Objects/MaterialConverter.h"
#include "PickupItem/HomingItem.h"
#include "Objects/TrashSpawningPool.h"
#include "Weapon/TrashCan.h"
#include "Components/WidgetInteractionComponent.h"
#include "HUD/BaseCharacterWidget.h"
#include "HUD/GameOver.h"
#include "Project_CrossBridge/Project_CrossBridgeGameModeBase.h"
#include "Spectator/PCSpectatorPawn.h"
#include "VRCharacter/VRComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "VRCharacter/Widget/VRGameOverWidget.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 100.f;
	springArmComp->bUsePawnControlRotation = true;

	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetupAttachment(springArmComp);

	overHeadComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("overHeadComp"));
	overHeadComp->SetupAttachment(RootComponent);
	overHeadComp->SetCollisionProfileName(FName("NoCollision"));
	overHeadComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DashEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashEffectComponent"));
	DashEffectComponent->SetAutoActivate(false);

	// 현재 Bridge 맵의 지역의 위치가 이상하다. 좌표는 0,0,0 인데 , 지역은 다른곳에 있다. 아무래도 중심좌표를 정확히 맞추지 않아서 그런것 같다.
	// 그래서 TraceLength를 굉장히 높게 설정해야 정확히 조준된다.
	TraceLength = 500, 000.f;

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> dashEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/Assets/effects/Niagara/NG_DashEffect.NG_DashEffect'"));

	if (dashEffect.Succeeded())
	{
		DashEffectComponent->SetAsset(dashEffect.Object);
	}

	DashEffectComponent->SetupAttachment(GetMesh());

	MaxHP = 100.f;
	CurrentHP = MaxHP;

	/** PC 캐릭터 디폴트 세팅 */
	GetCharacterMovement()->Crouch(true);
	GetCharacterMovement()->MaxWalkSpeed = 100;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 50;
	GetCharacterMovement()->MaxFlySpeed = 100.f;
	GetCapsuleComponent()->SetWorldScale3D(FVector(0.2f));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VRCameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraRoot"));
	VRCameraRoot->SetupAttachment(RootComponent);
	VRCameraRoot->SetIsReplicated(true);

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VRCameraRoot);
	//VRCamera->bUsePawnControlRotation = true;
	VRCamera->SetActive(false);
	VRCamera->SetIsReplicated(true);

	VRHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VRHeadMesh"));
	VRHeadMesh->SetupAttachment(VRCamera);
	VRHeadMesh->SetIsReplicated(true);

	HeadComp = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadComp"));
	HeadComp->SetupAttachment(VRHeadMesh);
	HeadComp->SetRelativeLocation(FVector(0, 11.8f, 58));
	HeadComp->SetBoxExtent(FVector(60, 90, 70));
	HeadComp->SetIsReplicated(true);

	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(VRCameraRoot);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	LeftHand->SetIsReplicated(true);

	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(VRCameraRoot);
	RightHand->SetTrackingMotionSource(FName("Right"));
	RightHand->SetIsReplicated(true);

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
	RightAim->SetupAttachment(VRCameraRoot);
	RightAim->SetTrackingMotionSource(FName("RightAim"));
	RightAim->SetIsReplicated(true);

	LeftHandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandBox"));
	LeftHandBox->SetupAttachment(LeftHand);
	LeftHandBox->SetRelativeLocation(FVector(3, 1, -4));
	LeftHandBox->SetBoxExtent(FVector(5));
	LeftHandBox->SetIsReplicated(true);

	RightHandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandBox"));
	RightHandBox->SetupAttachment(RightHand);
	RightHandBox->SetRelativeLocation(FVector(3, -1, -4));
	RightHandBox->SetBoxExtent(FVector(5));
	RightHandBox->SetIsReplicated(true);

	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
	LeftHandMesh->SetIsReplicated(true);

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);
	RightHandMesh->SetIsReplicated(true);

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(RightHand);
	SwordMesh->SetRelativeLocation(FVector(10.0f, -2.6f, -6.5f));
	SwordMesh->SetRelativeRotation(FRotator(-75, 0, 0));
	SwordMesh->SetRelativeScale3D(FVector(0.12f));
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordMesh->SetVisibility(false);
	SwordMesh->SetIsReplicated(true);

	InvisibleSwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvisibleSwordMesh"));
	InvisibleSwordMesh->SetupAttachment(RightHand);
	InvisibleSwordMesh->SetRelativeLocation(FVector(10.0f, -2.6f, -6.5f));
	InvisibleSwordMesh->SetRelativeRotation(FRotator(-75, 0, 0));
	InvisibleSwordMesh->SetRelativeScale3D(FVector(0.12f));
	InvisibleSwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InvisibleSwordMesh->SetVisibility(false);
	InvisibleSwordMesh->SetIsReplicated(true);

	SwordComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordComp"));
	SwordComp->SetupAttachment(SwordMesh);
	SwordComp->SetCollisionProfileName(TEXT("NoCollision"));
	SwordComp->SetRelativeLocation(FVector(-13, 0, 420));
	SwordComp->SetBoxExtent(FVector(13, 13, 370));
	SwordComp->SetIsReplicated(true);

	GrabbableObjectCreateEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GrabbableObjectCreateEffect"));
	GrabbableObjectCreateEffect->SetupAttachment(LeftHand);
	GrabbableObjectCreateEffect->SetRelativeLocation(FVector(3, 4, -6));
	GrabbableObjectCreateEffect->SetVisibility(false);
	GrabbableObjectCreateEffect->SetIsReplicated(true);

	BlackholeTraceComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BlackholeTraceComp"));
	BlackholeTraceComp->SetupAttachment(RootComponent);

	BulletAimTraceComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BulletAimTraceComp"));
	BulletAimTraceComp->SetupAttachment(RootComponent);

	LeftVRHealEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LeftVRHealEffect"));
	LeftVRHealEffect->SetupAttachment(LeftHand);
	LeftVRHealEffect->SetRelativeLocation(FVector(3, 4, -6));
	LeftVRHealEffect->SetRelativeRotation(FRotator(90, 0, 90));
	LeftVRHealEffect->SetVisibility(false);
	LeftVRHealEffect->SetIsReplicated(true);

	RightVRHealEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RightVRHealEffect"));
	RightVRHealEffect->SetupAttachment(RightHand);
	RightVRHealEffect->SetRelativeLocation(FVector(3, -4, -6));
	RightVRHealEffect->SetRelativeRotation(FRotator(0, 0, -90));
	RightVRHealEffect->SetVisibility(false);
	RightVRHealEffect->SetIsReplicated(true);

	VRStatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("VRStatus"));
	VRStatusWidget->SetupAttachment(LeftHand);
	VRStatusWidget->SetRelativeLocation(FVector(-2, 0.15, 6));
	VRStatusWidget->SetRelativeRotation(FRotator(8, 92, -70));
	VRStatusWidget->SetRelativeScale3D(FVector(0.025));
	VRStatusWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftWidgetInteraction = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWidgetInteraction"));
	LeftWidgetInteraction->SetupAttachment(LeftHand);
	LeftWidgetInteraction->SetRelativeLocation(FVector(9.8f, 4.1f, -13.1f));
	LeftWidgetInteraction->SetBoxExtent(FVector(1));
	LeftWidgetInteraction->SetIsReplicated(true);
	LeftWidgetInteraction->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftWidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LeftWidgetInteractionComp"));
	LeftWidgetInteractionComp->SetupAttachment(LeftAim);
	LeftWidgetInteractionComp->PointerIndex = 0;
	LeftWidgetInteractionComp->InteractionDistance = 15;

	RightWidgetInteraction = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWidgetInteraction"));
	RightWidgetInteraction->SetupAttachment(RightHand);
	RightWidgetInteraction->SetRelativeLocation(FVector(9.8f, -4.1f, -13.1f));
	RightWidgetInteraction->SetBoxExtent(FVector(1));
	RightWidgetInteraction->SetIsReplicated(true);
	RightWidgetInteraction->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightWidgetInteractionComp"));
	RightWidgetInteractionComp->SetupAttachment(RightAim);
	RightWidgetInteractionComp->PointerIndex = 1;
	RightWidgetInteractionComp->InteractionDistance = 15;

	

	ConstructorHelpers::FObjectFinder<UStaticMesh> VRHeadAsset(TEXT("//Script/Engine.StaticMesh'/Game/HYY/Download/Helmet/source/casco/cscf.cscf'"));
	if (VRHeadAsset.Succeeded())
	{
		VRHeadMesh->SetStaticMesh(VRHeadAsset.Object);
		VRHeadMesh->SetRelativeLocation(FVector(-22.7f, 0, -17.1f));
		VRHeadMesh->SetRelativeRotation(FRotator(0, -90, 0));
		VRHeadMesh->SetRelativeScale3D(FVector(0.3f));
		VRHeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> LeftMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (LeftMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(LeftMesh.Object);
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
	}

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
	LeftWidgetInteraction->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnWidgetLeftOverlap);
	LeftWidgetInteraction->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnWidgetLeftEndOverlap);
	RightWidgetInteraction->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnWidgetRightOverlap);
	RightWidgetInteraction->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnWidgetRightEndOverlap);

	/** PC 디폴트 설정*/
	VRHeadMesh->SetVisibility(false);
	HeadComp->SetCollisionProfileName(FName("NoCollision"));

	LeftHand->SetCollisionProfileName(FName("NoCollision"));
	RightHand->SetCollisionProfileName(FName("NoCollision"));

	LeftHandMesh->SetVisibility(false);
	LeftHandMesh->SetCollisionProfileName(FName("NoCollision"));

	RightHandMesh->SetVisibility(false);
	RightHandMesh->SetCollisionProfileName(FName("NoCollision"));

	LeftGrip->SetCollisionProfileName(FName("NoCollision"));
	RightAim->SetCollisionProfileName(FName("NoCollision"));
	LeftAim->SetCollisionProfileName(FName("NoCollision"));
	LeftHandBox->SetCollisionProfileName(FName("NoCollision"));
	RightHandBox->SetCollisionProfileName(FName("NoCollision"));

	VRStatusWidget->SetVisibility(false);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	BridgeState = Cast<ACrossBridgeStateBase>(GetWorld()->GetGameState());

	// 게임 시작시 플레이어 오버헤드(overhead)컬러 설정
	linearColor = FMath::VRand();
	linearColor = linearColor.GetAbs();

	// SetCurrentHealth(MaxHP);

	PCController = PCController == nullptr ? Cast<ABaseCharacterController>(GetController()) : PCController;

	if (PCController)
	{
		PCController->SetHealthStatus(CurrentHP, MaxHP);
		PCController->SetJetpackStatus(Fuel, MaxFuel);
	}

	// Health 초기화.
	if (HasAuthority())
	{
		SetCurrentHealth(MaxHP);
		OnRep_SetOverheadCompColor();
	}

	if (BridgeState != nullptr)
	{
		BridgeState->endStateDelegate.AddDynamic(this, &ABaseCharacter::ShowGameOverWidget);
		BridgeState->endStateDelegate.AddDynamic(this, &ABaseCharacter::VRShowGameOverWidget);
	}

	// UE_LOG(LogTemp,Warning,TEXT("Current HP : %.1f"),CurrentHP);

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
		//UE_LOG(LogTemp, Warning, TEXT("my Controller"));

		// Player Name
		GameInstance = GetGameInstance();

		if (GameInstance != nullptr)
		{
			MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

			if (MultiplayerSessionsSubsystem)
			{
				// ServerSetName(MultiplayerSessionsSubsystem->SessionID.ToString());
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IsVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	if (IsVR && !IsDummy)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		FTimerHandle VRSettingTimer;
		GetWorld()->GetTimerManager().SetTimer(VRSettingTimer, this, &ABaseCharacter::ServerVRSetting, 0.1f, false);
		// ServerVRSetting();
		camComp->SetActive(false);
		VRCamera->SetActive(true);

		RedDot = GetWorld()->SpawnActor<AActor>(SpawnRedDot, GetActorLocation(), GetActorRotation());
		RedDot->SetActorHiddenInGame(true);
		RedDot->SetActorEnableCollision(false);
		VRController = UGameplayStatics::GetPlayerController(this, 0);
		VRTimerController = Cast<ABaseCharacterController>(VRController);
		VRCamera->PostProcessSettings.WeightedBlendables.Array[1].Weight = 0;
	
	}

	VRStatus = Cast<UVRStatusWidget>(VRStatusWidget->GetWidget());
	VRStatus->SetHPBar(0);
	UMaterialInterface *HeadBase = VRHeadMesh->GetMaterial(0);
	if (HeadBase)
	{
		HeadMat = VRHeadMesh->CreateDynamicMaterialInstance(0, HeadBase);
		HeadMat->SetVectorParameterValue(FName("HeadColor"), (FLinearColor)FVector(0, 0.8, 0));
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
	UMaterialInterface *SwordBase = SwordMesh->GetMaterial(1);
	if (SwordBase)
	{
		SwordMat = SwordMesh->CreateDynamicMaterialInstance(1, SwordBase);
		SwordMat->SetScalarParameterValue(FName("SwordOpacity"), 0);
	}
	if (DamageEffectBase)
	{
		DamageEffectMat = UMaterialInstanceDynamic::Create(DamageEffectBase, this);
		DamageEffectMat->SetScalarParameterValue(FName("DamageRate"), 0);
		VRCamera->PostProcessSettings.WeightedBlendables.Array[0].Object = DamageEffectMat;
		VRCamera->PostProcessSettings.WeightedBlendables.Array[0].Weight = 1;
	}
	// UMaterialInterface *InvisibleSwordBase = InvisibleSwordMesh->GetMaterial(3);
	// if (InvisibleSwordBase)
	//{
	//	InvisibleSwordMat = InvisibleSwordMesh->CreateDynamicMaterialInstance(3, InvisibleSwordBase);
	//	InvisibleSwordMat->SetScalarParameterValue(FName("SwordOpacity"), 0);
	// }

	ServerResetColorChange(FString("SwordDissolve"));
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (bJetPackActive)
	{
		// DeltaTime을 통해 게임, 컴퓨터 성능에 따른 다른 이동속도를 갖지 못하도록 했다..
		AddMovementInput(FVector(0, 0, JetPackSpeed * DeltaTime));
	}

	if (PCController)
	{
		PCController->SetHealthStatus(CurrentHP, MaxHP);
		PCController->SetJetpackStatus(Fuel, MaxFuel);
		FreezeCoolDown(DeltaTime);
	}

	if (freeze != nullptr)
	{
		freeze->SetActorLocation(GetActorLocation());
	}

	if (!IsVR)
	{
		TraceUnderCosshairs(HitResult);
		SetHUDCrosshairs(DeltaTime);
	}

	if (IsVR)
	{
	
		ServerVRTransform(VRCamera->GetRelativeTransform(), LeftHand->GetRelativeTransform(), RightHand->GetRelativeTransform());

		float AimDotProduct = FVector::DotProduct(RightAim->GetForwardVector(), LeftAim->GetForwardVector());
		float GripDotProduct = FVector::DotProduct(RightGrip->GetRightVector(), LeftGrip->GetRightVector());

		float HandDist = FVector::Distance(LeftHand->GetComponentLocation(), RightHand->GetComponentLocation());

		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%f"), VRHeadMesh->GetComponentLocation().Z));
		if (AimDotProduct >= 0.9 && GripDotProduct >= 0.9 && VRSkillCheck(FString("Hands")) && HandDist >= 13 && HandDist <= 25)
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
						ServerHealEffectActivate(true);
						IsHeal = true;
						VRController->PlayHapticEffect(HealHaptic, EControllerHand::Right, 1, true);
						VRController->PlayHapticEffect(HealHaptic, EControllerHand::Left, 1, true);
					}
					VRGetDamage(-3 * DeltaTime);
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
					ServerHealEffectActivate(false);
					ServerResetColorChange("Right");
					VRCamera->PostProcessSettings.WeightedBlendables.Array[1].Weight = 0;
				}
			}
		}
		else if ((AimDotProduct < 0.9 || GripDotProduct < 0.9 || HandDist < 13 || HandDist > 25 || !VRSkillCheck(FString("Hands"))) && VRHealTime > 0)
		{
			VRHealTime = 0;
			if (IsHeal)
			{
				IsHeal = false;
				VRController->StopHapticEffect(EControllerHand::Right);
				VRController->StopHapticEffect(EControllerHand::Left);
				ServerHealEffectActivate(false);
				ServerResetColorChange("Right");
				VRCamera->PostProcessSettings.WeightedBlendables.Array[1].Weight = 0;
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

		if (VRCurHP <= 0 && !IsVRDead)
		{
			IsVRDead = true;
			ServerVRDeath(true);
			// 2초
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("Dead!!"));
			VRCamera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
			DisableInput(VRController);
			if (IsRightA)
			{
				RightAEnd();
			}
			// FTimerHandle VRReviveTimer;
			// GetWorld()->GetTimerManager().SetTimer(VRReviveTimer, this, &ABaseCharacter::VRRevive, VRReviveLimitTime, false);
		}
		else if (VRCurHP <= 0 && IsVRDead)
		{
			VRReviveTime += DeltaTime;
			if (VRReviveTime > 2 && VRReviveLimitTime > VRReviveTime)
			{
				if (!IsVRRevive)
				{
					IsVRRevive = true;
					VRHeadLoc = VRHeadActor->GetActorLocation();
					LeftHandLoc = VRLeftHandActor->GetActorLocation();
					RightHandLoc = VRRightHandActor->GetActorLocation();

					VRHeadRot = VRHeadActor->GetActorRotation();
					LeftHandRot = VRLeftHandActor->GetActorRotation();
					RightHandRot = VRRightHandActor->GetActorRotation();
				}
				float Rate = (VRReviveTime - 2) / (VRReviveLimitTime - 2);
				float VRate = 1.3f;

				if (Rate * VRate <= 1)
				{
					HeadReviveLoc = UKismetMathLibrary::VLerp(VRHeadLoc, VRHeadMesh->GetComponentLocation(), Rate * VRate);
					LeftHandReviveLoc = UKismetMathLibrary::VLerp(LeftHandLoc, LeftHandMesh->GetComponentLocation(), Rate * VRate);
					RightHandReviveLoc = UKismetMathLibrary::VLerp(RightHandLoc, RightHandMesh->GetComponentLocation(), Rate * VRate);
				}
				FRotator HeadReviveRot = UKismetMathLibrary::RLerp(VRHeadRot, VRHeadMesh->GetComponentRotation(), Rate, true);
				FRotator LeftHandReviveRot = UKismetMathLibrary::RLerp(LeftHandRot, LeftHandMesh->GetComponentRotation(), Rate, true);
				FRotator RightHandReviveRot = UKismetMathLibrary::RLerp(RightHandRot, RightHandMesh->GetComponentRotation(), Rate, true);
				ServerVRRevive(Rate, HeadReviveLoc, LeftHandReviveLoc, RightHandReviveLoc, HeadReviveRot, LeftHandReviveRot, RightHandReviveRot);
			}
			else if (VRReviveLimitTime <= VRReviveTime)
			{
				ServerVRReviveSetting();
				MulticastVRReviveSetting();
			}
		}

		if (VRTimerController)
		{
			VRStatus->SetTimerText(VRTimerController->VRTimer);
		}
	}

	SetGrabInfo();

	if (IsLeftY && !IsTrashSpawningPoolCool && !IsTrashSpawningPoolSet)
	{
		// if (!TrashSpawningPool)
		//{
		//	TrashSpawningPool = Cast<ATrashSpawningPool>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrashSpawningPool::StaticClass()));
		// }
		if (LeftYTimer == 0)
		{
			VRController->PlayHapticEffect(TrashCastHaptic, EControllerHand::Left, 1, true);
			IsTrashSpawningPoolCast = true;
		}
		LeftYTimer += DeltaTime;
		if (LeftYTimer / LeftYCastTime <= 1)
		{
			// ColorChange(LeftYTimer / 5, FString("Left"));
			ServerColorChange(LeftYTimer / LeftYCastTime, FString("LeftY"));
		}
		else
		{
			if (IsTrashSpawningPoolCast)
			{
				VRController->StopHapticEffect(EControllerHand::Left);
				IsTrashSpawningPoolCast = false;
				TrashSpawningPool->IsRangeActivate = true;
			}
			FVector Loc = LeftHand->GetComponentLocation() + LeftAim->GetForwardVector() * 1000;
			ServerTrashSpawningPoolSet(FVector(Loc.X, Loc.Y, 800));
		}
	}
	if (IsTrashSpawningPoolSet)
	{
		TrashSpawningPoolTime += DeltaTime;
		if (TrashSpawningPoolTime > TrashSpawningPoolTimeLimit)
		{
			TrashSpawningPoolTime = 0;
			ServerTrashSpawningPoolReset();
			IsTrashSpawningPoolSet = false;
			IsTrashSpawningPoolCool = true;
			TrashSpawningPool->IsRangeActivate = false;
			TrashSpawningPool->TrashRangeDeactivate();
		}
	}

	if (IsTrashSpawningPoolCool)
	{
		if (TrashSpawningPoolCoolTime == 0)
		{
			VRStatus->SetImageColor(FString("Trash"), true);
		}
		TrashSpawningPoolCoolTime += DeltaTime;
		VRStatus->SetCooltimeText(FString("Trash"), TrashSpawningPoolCoolTimeLimit - TrashSpawningPoolCoolTime);
		if (TrashSpawningPoolCoolTime > TrashSpawningPoolCoolTimeLimit)
		{
			VRStatus->SetCooltimeText(FString("Trash"), 0);
			VRStatus->SetImageColor(FString("Trash"), false);
			IsTrashSpawningPoolCool = false;
			TrashSpawningPoolCoolTime = 0;
		}
	}

	if (IsLeftX && !IsBlackholeCool && !IsBlackholeSet)
	{
		// if (!Blackhole)
		//{
		//	Blackhole = Cast<ABlackhole>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlackhole::StaticClass()));
		// }
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
				ServerVRSoundPlay(FString("SwordCast"));
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
				MulticastVRSoundPlay(FString("SwordCastStop"));
				VRController->StopHapticEffect(EControllerHand::Right);
				VRController->StopHapticEffect(EControllerHand::Left);
				SwordSetTime = 0;
				// ServerResetColorChange(FString("SwordOpacity"));
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
		if (SwordDamageCoolTime == 0)
		{
			SwordMat->SetVectorParameterValue(FName("MainColor"), (FLinearColor)FVector(0, 2.56f, 3));
			FTimerHandle SwordMatTimer;
			GetWorld()->GetTimerManager().SetTimer(SwordMatTimer,
												   FTimerDelegate::CreateLambda([this]() -> void
																				{ SwordMat->SetVectorParameterValue(FName("MainColor"), (FLinearColor)FVector(0, 0.256f, 0.3f)); }),
												   0.03, false);
		}
		SwordDamageCoolTime += DeltaTime;
		if (SwordDamageCoolTime > SwordDamageCoolTimeLimit)
		{
			SwordMat->SetVectorParameterValue(FName("MainColor"), (FLinearColor)FVector(0, 2.56f, 3));
			FTimerHandle SwordMatTimer;
			GetWorld()->GetTimerManager().SetTimer(SwordMatTimer,
												   FTimerDelegate::CreateLambda([this]() -> void
																				{ SwordMat->SetVectorParameterValue(FName("MainColor"), (FLinearColor)FVector(0, 25.6f, 30)); }),
												   0.03, false);
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
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%f"), SwordCoolTime));
		if (SwordCoolTime > SwordCoolTimeLimit)
		{
			VRStatus->SetCooltimeText(FString("Sword"), 0);
			VRStatus->SetImageColor(FString("Sword"), false);
			IsSwordCool = false;
			SwordCoolTime = 0;
		}
	}

	if (VRDamageEffectRate > 0)
	{
		VRDamageEffectRate -= 0.66f * DeltaTime;
		if (VRDamageEffectRate < 0)
		{
			VRDamageEffectRate = 0;
		}
		DamageEffectMat->SetScalarParameterValue(FName("DamageRate"), VRDamageEffectRate);
	}
	// FVector StartPos = LeftGrip->GetComponentLocation() + LeftAim->GetForwardVector() * 10;
	// FVector EndPos = StartPos + LeftGrip->GetRightVector() * 1000;
	// FVector StartPos = LeftHand->GetComponentLocation();
	// FVector EndPos = StartPos + LeftHand->GetUpVector() * 1000;
	// DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, -1, 0, 1);
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
		EnhancedInputComponent->BindAction(InputInterAction, ETriggerEvent::Started, this, &ABaseCharacter::CanonFire);
		EnhancedInputComponent->BindAction(InputInterAction, ETriggerEvent::Started, this, &ABaseCharacter::UsingConverter);
		EnhancedInputComponent->BindAction(InputTrashCanFireAction, ETriggerEvent::Started, this, &ABaseCharacter::TrashCanFire);

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
	// UE_LOG(LogTemp, Warning, TEXT("Base Attack"));
	Fire();
}

void ABaseCharacter::ContextualActionPressed()
{
	// UE_LOG(LogTemp, Warning, TEXT("Base ContextualAction Pressed"));
	FreezeSpawn();
}

void ABaseCharacter::ContextualActionReleased()
{
	// UE_LOG(LogTemp, Warning, TEXT("Base ContextualAction Released"));
	RemoveFreeze();
}

/** PC Player Overhead Comp Color */
#pragma region OverheadComp
void ABaseCharacter::OnRep_SetOverheadCompColor()
{

	UMaterialInterface *iMat = overHeadComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("MyColor"));

	iMat->GetVectorParameterValue(param, initColor);

	dynamicMat = UMaterialInstanceDynamic::Create(iMat, this);

	if (dynamicMat != nullptr)
	{
		dynamicMat->SetVectorParameterValue(TEXT("MyColor"), linearColor);
		overHeadComp->SetMaterial(0, dynamicMat);
	}
}
#pragma endregion

/** Sliding , Rolling Functions()*/
#pragma region Sliding, Rolling Function()

void ABaseCharacter::RollingActionPressed()
{
	if (freeze == nullptr && !bIsAction)
	{
		bIsAction = true;
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
	bIsAction = false;
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ABaseCharacter::SlidingActionPressed()
{
	if (freeze == nullptr && !bIsAction)
	{
		bIsAction = true;
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
	// GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	if (SlidingMontage)
	{
		PlayAnimMontage(SlidingMontage);
	}
	GetCapsuleComponent()->SetCapsuleHalfHeight(20.f);
}

void ABaseCharacter::Multicast_SlidingActionReleased_Implementation()
{
	UnCrouch();
	bIsAction = false;
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
	if (freeze == nullptr && !bIsDead)
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
	if (freeze == nullptr && !bIsDead)
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

void ABaseCharacter::Server_RecoveryHP_Implementation(float value)
{
	float hp = CurrentHP + value;
	SetCurrentHealth(hp);
}

void ABaseCharacter::SetCurrentHealth(float value)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHP = FMath::Clamp(value, 0.f, MaxHP);

		OnHealthUpdate();
	}
}

void ABaseCharacter::OnTakeDamage(float d)
{
	if(freeze == nullptr)
	{
		float hp = CurrentHP - d;
		SetCurrentHealth(hp);
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%f"), CurrentHP));
		Multicast_Hit();
	}
}

void ABaseCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ABaseCharacter::OnHealthUpdate()
{
	// 클라이언트 전용 함수 기능
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHP);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHP <= 0)
		{
			bIsDead = true;
			// PCPlayerDead();
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	// 서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHP);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHP <= 0)
		{
			bIsDead = true;
			// PCPlayerDead();
			// FString deathMessage = FString::Printf(TEXT("You have been killed."));
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	// 모든 머신에서 실행되는 함수
	/*
		여기에 대미지 또는 사망의 결과로 발생하는 특별 함수 기능 배치
	*/

	if (PCController)
	{
		PCController->SetHealthStatus(CurrentHP, MaxHP);
	}

	if (bIsDead)
	{
		MultiCast_PCPlayerDead();
	}
}

#pragma endregion

/** Player Dead , Respawn*/
#pragma region PCPlayer Dead, Respawn

void ABaseCharacter::PCPlayerDead()
{
	if (GetController() != nullptr && GetController()->IsLocalController())
	{
		GetCharacterMovement()->DisableMovement();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bUseControllerRotationYaw = false;
		camComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
	}
}

void ABaseCharacter::Multicast_Hit_Implementation()
{
	if (HitMontage != nullptr)
	{
		PlayAnimMontage(HitMontage);
		if(BloodEffect)
		{
			const USkeletalMeshSocket* bloodSocket = GetMesh()->GetSocketByName(FName("BloodSocket"));
			UNiagaraFunctionLibrary::SpawnSystemAttached(BloodEffect,GetMesh(),FName("BloodSocket"),GetMesh()->GetSocketLocation(FName("BloodSocket")),GetMesh()->GetSocketRotation(FName("BloodSocket")),EAttachLocation::KeepWorldPosition,true);
		}
	}
}

void ABaseCharacter::Server_PCPlayerDead_Implementation()
{
	if (GetController() != nullptr && GetController()->IsLocalController())
	{
		GetCharacterMovement()->DisableMovement();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bUseControllerRotationYaw = false;
		camComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
	}
}

void ABaseCharacter::MultiCast_PCPlayerDead_Implementation()
{
	FString DeadMessage = FString::Printf(TEXT("Dead"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, DeadMessage);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bUseControllerRotationYaw = false;
	camComp->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
}

void ABaseCharacter::ChangeSpectator()
{
	if (HasAuthority())
	{
		SpawnTrashCan();
		AProject_CrossBridgeGameModeBase *GM = Cast<AProject_CrossBridgeGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GM != nullptr)
		{
			GM->SetDeadTransform(GetActorLocation(), GetActorRotation());
			TSubclassOf<APCSpectatorPawn> spectatorPawn = GM->SpectatorClass;

			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			APCSpectatorPawn *spectator = GetWorld()->SpawnActor<APCSpectatorPawn>(
				spectatorPawn,
				GetActorLocation(),
				GetActorRotation(),
				param);

			if (spectator != nullptr)
			{
				spectator->SetOrigin(this);
				GetController()->Possess(spectator);
				Destroy();
			}
		}
	}
}
#pragma endregion

/** Fire */
#pragma region Fire()

void ABaseCharacter::TrashCanFire()
{
	if (myTrashCan != nullptr && freeze == nullptr)
	{
		if (myTrashCan->GetbFireDelay())
		{
			myTrashCan->Fire(this, HitTarget);
		}
	}
}

void ABaseCharacter::Fire()
{
	if (myWeapon != nullptr && freeze == nullptr && bisEquip)
	{
		if (myWeapon->GetbFireDelay())
		{
			Server_Fire();
			myWeapon->Fire(this, HitTarget);
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
		// UE_LOG(LogTemp, Warning, TEXT("Fire Montage"));
		PlayAnimMontage(fireMontage);
	}
}
#pragma endregion

/** 쓰레기통을 들고 죽는다면 쓰레기통 소환.. */
void ABaseCharacter::SpawnTrashCan()
{
	float count = 0.f;
	if (myTrashCan != nullptr)
	{
		count = myTrashCan->GetCount();
		myTrashCan->Destroy();
		if (trashcanFactory)
		{
			ATrashCan *Trash = GetWorld()->SpawnActor<ATrashCan>(trashcanFactory, GetActorLocation(), GetActorRotation());
			Trash->SetCount(count);
		}
	}
}

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

/** Freeze Skill */
#pragma region Skill Freeze
void ABaseCharacter::FreezeSpawn()
{
	Server_FreezeSpawn();
}

void ABaseCharacter::Server_FreezeSpawn_Implementation()
{
	if(bCanFreeze)
	{
		if(FreezeFactory != nullptr)
		{
			freeze = GetWorld()->SpawnActor<AFreeze>(FreezeFactory, GetActorLocation(), GetActorRotation());
			if (freeze != nullptr)
			{
				freeze->SetOwner(this);
				bCanFreeze = false;
			}

			GetCharacterMovement()->DisableMovement();
		}
	}

}

void ABaseCharacter::RemoveFreeze()
{
	Server_RemoveFreeze();
}



void ABaseCharacter::Server_RemoveFreeze_Implementation()
{
	if(freeze != nullptr)
	{
		freeze->Destroy();
		if (freeze != nullptr)
		{
			//coolTime = FreezeCoolTime;
			Multicast_RemoveFreeze();
			freeze = nullptr;
		}

		FTimerHandle freezeHandle;
		GetWorldTimerManager().SetTimer(freezeHandle,FTimerDelegate::CreateLambda([&]()->void
		{
			bCanFreeze = true;
		}),FreezeCoolTime,false);
		DeActivateJetPack();
	}

}

void ABaseCharacter::Multicast_RemoveFreeze_Implementation()
{
	coolTime = FreezeCoolTime;
}

void ABaseCharacter::FreezeCoolDown(float time)
{
	if(freeze == nullptr && coolTime >=0 )
	{
		coolTime -= time;
		if(PCController)
		{
			PCController->SetFreezeStatus(coolTime,FreezeCoolTime);
		}
	}
	
}


#pragma endregion

#pragma region Canon
void ABaseCharacter::CanonFire()
{
	if (mycanon != nullptr)
	{
		if (myHoming != nullptr)
		{
			mycanon->Reload(myHoming);

			myHoming->UsingItem(this);
			if (myHoming != nullptr)
			{
				myHoming = nullptr;
			}
		}
		else
		{
			if (mycanon->GetFireDelay())
			{
				mycanon->HomingFire(this);
			}
		}
	}
}

#pragma endregion

#pragma region SpeedUp
void ABaseCharacter::SpeedUp()
{
	if (!bIsSpeedUp)
	{
		MultiCast_SpeedUP();
	}
}

void ABaseCharacter::MultiCast_SpeedUP_Implementation()
{
	DashEffectComponent->SetActive(true); // 아이템먹으면 Effect 활성화
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;

	bIsSpeedUp = true;

	FTimerHandle SpeedUpDelayHandle;
	GetWorld()->GetTimerManager().SetTimer(SpeedUpDelayHandle, this, &ABaseCharacter::ComeBackSpeed, DuringSpeedTime);
}

void ABaseCharacter::ComeBackSpeed()
{
	DashEffectComponent->SetActive(false); // 아이템먹으면 Effect 비활성화
	MultiCast_CombackSpeed();
}

void ABaseCharacter::MultiCast_CombackSpeed_Implementation()
{
	if (!bIsSpeedUp)
	{
		return;
	}

	SpeedCurrentTime = 0.f;

	// UE_LOG(LogTemp,Warning,TEXT("ComeBackSpeed"));

	GetWorld()->GetTimerManager().SetTimer(SpeedHandle,
										   FTimerDelegate::CreateLambda([this]() -> void
																		{
			SpeedCurrentTime += GetWorld()->DeltaTimeSeconds;

			float curWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
			float curCrouchSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;

			float walkspeed = 100.f;
			float crouchSpeed = 50.f;

			curWalkSpeed = FMath::Lerp<float>(curWalkSpeed, walkspeed, SpeedCurrentTime/ReturnSpeedTime);
			curCrouchSpeed = FMath::Lerp<float>(curCrouchSpeed, crouchSpeed, SpeedCurrentTime/ReturnSpeedTime);

			GetCharacterMovement()->MaxWalkSpeed = curWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = curCrouchSpeed;

			if(SpeedCurrentTime >= ReturnSpeedTime)
			{
				GetCharacterMovement()->MaxWalkSpeed = walkspeed;
				GetCharacterMovement()->MaxWalkSpeedCrouched = crouchSpeed;
				bIsSpeedUp = false;
				GetWorld()->GetTimerManager().ClearTimer(SpeedHandle);
				//UE_LOG(LogTemp,Warning,TEXT("End Speed UP"));
			} }),
										   0.02f, true);
}

#pragma endregion

void ABaseCharacter::SetHUDCrosshairs(float DeletaTime)
{

	if (myWeapon == nullptr && myTrashCan == nullptr)
	{
		bDisplayCrosshair = false;
	}
	else
	{
		bDisplayCrosshair = true;
	}

	PCController = PCController == nullptr ? Cast<ABaseCharacterController>(GetController()) : PCController;

	if (PCController)
	{
		HUD = HUD == nullptr ? Cast<AWeaponHUD>(PCController->GetHUD()) : HUD;

		if (HUD)
		{
			FHUDStruct HudStruct;
			if (bDisplayCrosshair)
			{
				HudStruct.CrosshairCenter = CrosshairsCenter;
				HudStruct.CrosshairRight = CrosshairsRight;
				HudStruct.CrosshairLeft = CrosshairsLeft;
				HudStruct.CrosshairBottom = CrosshairsBottom;
				HudStruct.CrosshairTop = CrosshairsTop;
			}
			else
			{
				HudStruct.CrosshairCenter = nullptr;
				HudStruct.CrosshairRight = nullptr;
				HudStruct.CrosshairLeft = nullptr;
				HudStruct.CrosshairBottom = nullptr;
				HudStruct.CrosshairTop = nullptr;
			}
			HUD->SetHUDStruct(HudStruct);
		}
	}
}

void ABaseCharacter::TraceUnderCosshairs(FHitResult &TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation = ViewportSize * 0.5f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// 2D 스크린 좌표를 3D 월드 좌표로 변환
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
																   CrosshairLocation,
																   CrosshairWorldPosition,
																   CrosshairWorldDirection);

	// 변환 작업 성공
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = CrosshairWorldDirection * TraceLength;

		// 자기자신은 충돌에서 제외
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

		// LineTrace 범위안에 감지되는 액터가 존재하지 않을시..
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
			// DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint,12.f,12,FColor::Green);
		}
		else // 범위안에 감지되는 액터가 존재한다면..
		{
			HitTarget = TraceHitResult.ImpactPoint;
			// DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint,12.f,12,FColor::Red);
		}
	}
}

void ABaseCharacter::UsingConverter()
{
	if (myConverter != nullptr)
	{
		// 만약 쓰레기통을 들고 있다면..
		if (myTrashCan != nullptr)
		{
			// 쓰레기통속에 모아둔 쓰레기가 존재한다면..
			if (myTrashCan->GetCount() > 0)
			{
				// UE_LOG(LogTemp,Warning,TEXT("쓰레기 저장"));
				//  변환기에 쓰레기통에 존재하는 모든 쓰레기부터 저장한다..
				myConverter->SaveGarbage(myTrashCan->GetCount());

				Server_Fillup();
				
				// 쓰레기통에 존재하던 쓰레기 개수를 0으로 초기화한다.
				myTrashCan->SetCount(-1 * myTrashCan->GetCount());
			}
			else // 만약 존재하지 않는다면 바로 HomingItem을 소환한다.
			{
				// UE_LOG(LogTemp,Warning,TEXT("아이템 소환"));
				myConverter->MakingHoming();
			}
		}
		else // 만약 쓰레기통을 들고 있지 않다면..
		{
			// 바로 HomingItem을 소환한다.
			myConverter->MakingHoming();
		}
	}
}

void ABaseCharacter::Server_Fillup_Implementation()
{
	MultiCast_Fillup();
}

void ABaseCharacter::MultiCast_Fillup_Implementation()
{
	if(FillupMontage != nullptr)
	{
		PlayAnimMontage(FillupMontage);
	}
}

void ABaseCharacter::ShowGameOverWidget()
{
	// 게임이 끝나지 않았다면 위젯띄우기 종료
	if (BridgeState->GetGameState() != EGameState::End)
	{
		return;
	}

	if (GameOverWidget == nullptr)
	{
		return;
	}

	if (IsVR)
	{
		return;
	}

	if (GameOverMenu == nullptr)
	{

		GameOverMenu = CreateWidget<UGameOver>(GetWorld(), GameOverWidget);

		if (GameOverMenu != nullptr)
		{
			if (!bGameOverMenuOpen)
			{
				if (BridgeState->GetWinner() == EWinner::VR)
				{
					GameOverMenu->SetDisplayText(FString("LOSE"));
				}
				else if (BridgeState->GetWinner() == EWinner::PC)
				{
					GameOverMenu->SetDisplayText(FString("WIN"));
				}

				GameOverMenu->MenuSetup();
				bGameOverMenuOpen = true;
			}
		}
	}
}

void ABaseCharacter::VRShowGameOverWidget()
{
	if (BridgeState->GetGameState() != EGameState::End)
	{
		return;
	}

	if (!IsVR)
	{
		return;
	}

	if (!IsVRGameOver)
	{
		if (BridgeState->GetWinner() == EWinner::VR)
		{
			GetWorld()->SpawnActor<AActor>(VRWinActor, FVector(200, -100, 300), FRotator(0, 90, 0));
		}
		else if (BridgeState->GetWinner() == EWinner::PC)
		{
			GetWorld()->SpawnActor<AActor>(VRLoseActor, FVector(200, -100, 300), FRotator(0, 90, 0));
		}

		IsVRGameOver = true;
		ServerVRSetLocation(FVector(220, 405, 200));
		VRController->SetControlRotation(FRotator(0, 270, 0));
		DisableInput(VRController);
	}


}

void ABaseCharacter::ServerVRSetLocation_Implementation(FVector Loc)
{
	SetActorLocation(Loc);
}

void ABaseCharacter::Clinet_ShowGameOverWidget_Implementation()
{
	// 게임이 끝나지 않았다면 위젯띄우기 종료
	if (BridgeState->GetGameState() != EGameState::End)
	{
		return;
	}

	if (GameOverWidget == nullptr)
	{
		return;
	}

	if (IsVR)
	{
		return;
	}

	if (GameOverMenu == nullptr)
	{

		GameOverMenu = CreateWidget<UGameOver>(GetWorld(), GameOverWidget);

		if (GameOverMenu != nullptr)
		{
			if (!bGameOverMenuOpen)
			{
				if (BridgeState->GetWinner() == EWinner::VR)
				{
					GameOverMenu->SetDisplayText(FString("LOSE"));
				}
				else if (BridgeState->GetWinner() == EWinner::PC)
				{
					GameOverMenu->SetDisplayText(FString("WIN"));
				}

				GameOverMenu->MenuSetup();
				bGameOverMenuOpen = true;
			}
		}
	}
}

// 서버에 복제 등록하기 위한 함수
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bJetPackActive);
	DOREPLIFETIME(ABaseCharacter, Fuel);
	DOREPLIFETIME(ABaseCharacter, CurrentHP);
	DOREPLIFETIME(ABaseCharacter, freeze);
	DOREPLIFETIME(ABaseCharacter, IsBlackholeSet);
	DOREPLIFETIME(ABaseCharacter, VRCurHP);
	DOREPLIFETIME(ABaseCharacter, myHoming);
	DOREPLIFETIME(ABaseCharacter, Blackhole);
	DOREPLIFETIME(ABaseCharacter, TrashSpawningPool);
	DOREPLIFETIME(ABaseCharacter, myConverter);
	DOREPLIFETIME(ABaseCharacter, ReturnSpeedTime);
	DOREPLIFETIME(ABaseCharacter, DuringSpeedTime);
	DOREPLIFETIME(ABaseCharacter, bisEquip);
	DOREPLIFETIME(ABaseCharacter, linearColor);
	DOREPLIFETIME(ABaseCharacter, bIsDead);
	DOREPLIFETIME(ABaseCharacter, VRHeadActor);
	DOREPLIFETIME(ABaseCharacter, VRLeftHandActor);
	DOREPLIFETIME(ABaseCharacter, VRRightHandActor);
	DOREPLIFETIME(ABaseCharacter, bCanFreeze);
	DOREPLIFETIME(ABaseCharacter, VRDamageEffectRate);
	DOREPLIFETIME(ABaseCharacter, bIsAction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ABaseCharacter::ServerVRHeightSet_Implementation(bool IsUP)
{
	if (IsUP)
	{	
		VRCameraRoot->SetRelativeLocation(FVector(0, 0, 0));
		//RightAim->SetRelativeLocation(FVector(0, 0, ZLoc));
		float ZLoc = 40 + GetCapsuleComponent()->GetComponentLocation().Z - VRHeadMesh->GetComponentLocation().Z;
		VRCameraRoot->SetRelativeLocation(FVector(0, 0, ZLoc));
	}
	else
	{
		VRCameraRoot->SetRelativeLocation(FVector(0, 0, 0));
		float ZLoc = -20 + GetCapsuleComponent()->GetComponentLocation().Z - VRHeadMesh->GetComponentLocation().Z;
		VRCameraRoot->SetRelativeLocation(FVector(0, 0, ZLoc));
	}
}

void ABaseCharacter::VRMove(const FInputActionValue &Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddMovementInput(VRCamera->GetForwardVector(), Axis.X);
	AddMovementInput(VRCamera->GetRightVector(), Axis.Y);

}

void ABaseCharacter::Turn(const FInputActionValue &Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	if (IsVR)
	{
		if (Axis.X > 0.5f)
		{
			AddControllerYawInput(20);
		}
		else if (Axis.X < -0.5f)
		{
			AddControllerYawInput(-20);
		}
		if (Axis.Y > 0.5f)
		{
			//VRCameraRoot->SetRelativeLocation(VRCameraRoot->GetRelativeLocation() + FVector(0, 0, 10));
			//LeftHand->SetRelativeLocation(LeftHand->GetRelativeLocation() + FVector(0, 0, 10));
			//float ZLoc = 88 + GetCapsuleComponent()->GetComponentLocation().Z - VRHeadMesh->GetComponentLocation().Z > 88 ? 88 : 88 + GetCapsuleComponent()->GetComponentLocation().Z - VRHeadMesh->GetComponentLocation().Z;	
			ServerVRHeightSet(true);
		}
		else if (Axis.Y < -0.5f)
		{
			ServerVRHeightSet(false);
			//VRCameraRoot->SetRelativeLocation(VRCameraRoot->GetRelativeLocation() + FVector(0, 0, -10));
		}
	}

	// AddControllerYawInput(Axis.X);
	// AddControllerPitchInput(Axis.Y);
}

void ABaseCharacter::LeftIndexCurl()
{
	IsLeftIndexCurl = true;
	if (GrabbedActorLeft && IsLeftGrasp && !IsLeftGrab)
	{
		ServerGrabTheActor(GrabbedActorLeft, FString("Left"));
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left);
	}
}

void ABaseCharacter::LeftGrasp()
{
	IsLeftGrasp = true;
	if (GrabbedActorLeft && IsLeftIndexCurl && !IsLeftGrab)
	{
		ServerGrabTheActor(GrabbedActorLeft, FString("Left"));
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left);
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
		// ServerSpawnThunder();
		// TrashSpawningPool->SetActorLocation(GetActorLocation() + GetActorUpVector()*200);
		ServerTrashSpawningPoolActivate();
		IsTrashSpawningPoolSet = true;
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left, 1, false);
	}
	if (IsTrashSpawningPoolCast)
	{
		IsTrashSpawningPoolCast = false;
		VRController->StopHapticEffect(EControllerHand::Left);
	}
	IsLeftY = false;
	LeftYTimer = 0;
	// ResetColorChange(FString("Left"));
	ServerResetColorChange(FString("Left"));
}

void ABaseCharacter::LeftX()
{
	if (!IsLeftIndexCurl && !IsLeftGrasp && !IsLeftY && !IsBlackholeSet && !IsLeftX && !IsBlackholeCool && BlackholeTimer == 0)
	{
		IsLeftX = true;
		VRController->PlayHapticEffect(BlackholeCastHaptic, EControllerHand::Left, 1, false);
	}
}

void ABaseCharacter::LeftXEnd()
{
	if (IsLeftX && LeftXTimer < LeftXCastTime)
	{
		VRController->StopHapticEffect(EControllerHand::Left);
	}
	if (IsLeftX)
	{
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left, 1, false);
	}
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
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
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
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
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
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left);
		ServerUnGrabTheActor(GrabbedActorLeft, FString("Left"), RightThrowDir, RightThrowRot);
	}
}

void ABaseCharacter::LeftGraspEnd()
{
	//VRGetDamage(10);
	IsLeftGrasp = false;
	if (IsLeftGrab)
	{
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left);
		ServerUnGrabTheActor(GrabbedActorLeft, FString("Left"), RightThrowDir, RightThrowRot);
	}
}

void ABaseCharacter::RightIndexCurlEnd()
{
	IsRightIndexCurl = false;
	if (IsRightGrab)
	{
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
		ServerUnGrabTheActor(GrabbedActorRight, FString("Right"), RightThrowDir, RightThrowRot);
	}
}

void ABaseCharacter::RightGraspEnd()
{
	IsRightGrasp = false;
	if (IsRightGrab)
	{
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
		ServerUnGrabTheActor(GrabbedActorRight, FString("Right"), RightThrowDir, RightThrowRot);
	}
}

void ABaseCharacter::RightB()
{
	// VRGetDamage(1);
	if (!IsRightA && !IsRedDotSet && VRSkillCheck(FString("Right")))
	{
		IsRightB = true;
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("READY"));
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
			FTimerHandle SpawnBulletTimer1;
			GetWorld()->GetTimerManager().SetTimer(SpawnBulletTimer1,
				FTimerDelegate::CreateLambda([this]() -> void
					{
						FVector StartVec = RightAim->GetComponentLocation();
						FVector EndVec = StartVec + RightAim->GetForwardVector() * 1;
						FRotator Rot = RightAim->GetComponentRotation();
						ServerSpawnThrowingWeapon(EndVec, Rot);
						VRController->PlayHapticEffect(BulletFireHaptic, EControllerHand::Right);
					}), 0.25f, false);	
			FTimerHandle SpawnBulletTimer2;
			GetWorld()->GetTimerManager().SetTimer(SpawnBulletTimer2,
				FTimerDelegate::CreateLambda([this]() -> void
					{
						FVector StartVec = RightAim->GetComponentLocation();
						FVector EndVec = StartVec + RightAim->GetForwardVector() * 1;
						FRotator Rot = RightAim->GetComponentRotation();
						ServerSpawnThrowingWeapon(EndVec, Rot);
						VRController->PlayHapticEffect(BulletFireHaptic, EControllerHand::Right);
					}), 0.5f, false);
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
	BulletAimTraceComp->SetVisibility(false);
}

void ABaseCharacter::RightA()
{
	//VRGetDamage(10);
	if (!IsRightA && !IsSwordCool && VRSkillCheck(FString("Right")))
	{
		VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
		ServerColorChange(0, FString("SwordOpacity"));
		IsRightA = true;
	}
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
		SwordMat->SetVectorParameterValue(FName("MainColor"), (FLinearColor)FVector(0, 25.6f, 30));
	}
	IsRightA = false;
	SwordSetTime = 0;
	SwordActivateTime = 0;
	MulticastVRSoundPlay(FString("SwordCastStop"));
	VRController->StopHapticEffect(EControllerHand::Right);
	VRController->StopHapticEffect(EControllerHand::Left);
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
	if (VRSkillCheck(FString("Left")) && OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel10)
	{
		ServerSpawnGrabbableActor(OtherComp->GetOwner());
		ServerLeftHandActivate(false);
		FTimerHandle SpawnGrabbableActorCoolTimer;
		GetWorld()->GetTimerManager().SetTimer(SpawnGrabbableActorCoolTimer,
											   FTimerDelegate::CreateLambda([this]() -> void
																			{ ServerLeftHandActivate(true); }),
											   20, false);
	}
	if (OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		ServerVRDoorBreak(FString("Left"), OtherComp);
		// if (BridgeState->GetGameState() == EGameState::Wait)
		//{
		//	BridgeState->SetGameState(EGameState::Start);
		// }
		// GetWorld()->SpawnActor<AActor>(BreakDoor, LeftHand->GetComponentLocation(), LeftHand->GetComponentRotation());
	}
}
void ABaseCharacter::ServerLeftHandActivate_Implementation(bool IsActivate)
{
	MulticastLeftHandActivate(IsActivate);
}
void ABaseCharacter::MulticastLeftHandActivate_Implementation(bool IsActivate)
{
	if (IsActivate)
	{
		LeftHandBox->SetCollisionProfileName(TEXT("PlayerHandPreset"));
	}
	else
	{
		LeftHandBox->SetCollisionProfileName(TEXT("NoCollision"));
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
	if (OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		ServerVRDoorBreak(FString("Right"), OtherComp);

		// if (BridgeState->GetGameState() == EGameState::Wait)
		//{
		//	BridgeState->SetGameState(EGameState::Start);
		// }
	}
}

void ABaseCharacter::OnRightHandEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (GrabbedActorRight && GrabbedActorRight == Cast<ABaseGrabbableActor>(OtherActor))
	{
		GrabbedActorRight = NULL;
	}
}

void ABaseCharacter::OnSwordOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	class ABaseCharacter *Enemy = Cast<ABaseCharacter>(OtherActor);
	if (Enemy && !IsSwordDamageCool)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("IsHit"));
		IsSwordDamageCool = true;
		// Enemy->Server_TakeDamage(10);
		ServerVRAttack(FString("Sword"), Enemy);
	}
}

void ABaseCharacter::OnWidgetLeftOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	LeftWidgetInteractionComp->PressPointerKey(FKey(TEXT("LeftMouseButton")));
	VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Left);
}

void ABaseCharacter::OnWidgetLeftEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	LeftWidgetInteractionComp->ReleasePointerKey(FKey(TEXT("LeftMouseButton")));
}
void ABaseCharacter::OnWidgetRightOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	RightWidgetInteractionComp->PressPointerKey(FKey(TEXT("LeftMouseButton")));
	VRController->PlayHapticEffect(ClickedHaptic, EControllerHand::Right);
}

void ABaseCharacter::OnWidgetRightEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	RightWidgetInteractionComp->ReleasePointerKey(FKey(TEXT("LeftMouseButton")));
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

	FVector CurPos = LeftHand->GetComponentLocation() + LeftHand->GetUpVector() * -10 + LeftHand->GetForwardVector() * 5;
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
		// float OpacityRate = UKismetMathLibrary::Lerp(0.3, 1, Rate);
		// InvisibleSwordMat->SetScalarParameterValue(FName("SwordOpacity"), OpacityRate);
		InvisibleSwordMesh->SetVisibility(true);
	}
	else if (Position == FString("SwordDissolve"))
	{
		if (!SwordMesh->IsVisible())
		{
			SwordMesh->SetVisibility(true);
		}
		float DissolveRate = UKismetMathLibrary::Lerp(0, 1, Rate);
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
		// InvisibleSwordMat->SetScalarParameterValue(FName("SwordOpacity"), 0);
		InvisibleSwordMesh->SetVisibility(false);
	}
	else if (Position == FString("SwordDissolve"))
	{
		if (SwordMat)
		{
			SwordMat->SetScalarParameterValue(FName("Dissolve"), 0);
		}
		SwordMesh->SetVisibility(false);
	}
}
void ABaseCharacter::ServerSpawnGrabbableActor_Implementation(AActor *OtherActor)
{
	ABaseGrabbableActor *GrabActor = GetWorld()->SpawnActor<ABaseGrabbableActor>(SpawnGrabbedActor, LeftHand->GetComponentLocation(), LeftHand->GetComponentRotation());
	OtherActor->Destroy();
}

void ABaseCharacter::ServerVRSetting_Implementation()
{
	Blackhole = GetWorld()->SpawnActor<ABlackhole>(SpawnBlackhole, FVector(0, 0, -1000), FRotator(0, 0, 0));
	TrashSpawningPool = GetWorld()->SpawnActor<ATrashSpawningPool>(SpawnTrashSpawningPool, FVector(0, 0, -1000), FRotator(0, 0, 0));
	VRHeadActor = GetWorld()->SpawnActor<AVRComponent>(VRHeadSpawn, FVector(0, 0, -1000), FRotator(0, 0, 0));
	VRLeftHandActor = GetWorld()->SpawnActor<AVRComponent>(VRLeftHandSpawn, FVector(0, 0, -1000), FRotator(0, 0, 0));
	VRRightHandActor = GetWorld()->SpawnActor<AVRComponent>(VRRightHandSpawn, FVector(0, 0, -1000), FRotator(0, 0, 0));
	MulticastVRSetting();
}

void ABaseCharacter::MulticastVRSetting_Implementation()
{
	GetMesh()->SetVisibility(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// PC 애니메이션 제거
	GetMesh()->SetAnimClass(nullptr);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("VRPlayerPreset"));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.3f));

	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;
	GetCharacterMovement()->MaxFlySpeed = 600;

	VRHeadMesh->SetVisibility(true);
	HeadComp->SetCollisionProfileName(TEXT("VRPlayerHeadPreset"));

	LeftHandMesh->SetVisibility(true);
	RightHandMesh->SetVisibility(true);

	LeftHandBox->SetCollisionProfileName(TEXT("PlayerHandPreset"));
	RightHandBox->SetCollisionProfileName(TEXT("PlayerHandPreset"));

	VRStatusWidget->SetVisibility(true);

	LeftWidgetInteraction->SetCollisionProfileName(TEXT("VRPlayerWidgetInteractionPreset"));
	RightWidgetInteraction->SetCollisionProfileName(TEXT("VRPlayerWidgetInteractionPreset"));

	overHeadComp->SetVisibility(false);
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

void ABaseCharacter::ServerTrashSpawningPoolSet_Implementation(FVector Loc)
{
	if (GrabbableObjectCreateEffect->IsVisible())
	{
		GrabbableObjectCreateEffect->SetVisibility(false);
	}
	TrashSpawningPool->SetActorLocation(Loc);
}

void ABaseCharacter::ServerTrashSpawningPoolReset_Implementation()
{
	TrashSpawningPool->ServerDeactivate();
}

void ABaseCharacter::ServerVRTransform_Implementation(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform)
{
	// VRCamera->SetRelativeTransform(HeadTransform);
	// LeftHand->SetRelativeTransform(LeftTransform);
	// RightHand->SetRelativeTransform(RightTransform);
	MulticastVRTransform(HeadTransform, LeftTransform, RightTransform);
}

void ABaseCharacter::MulticastVRTransform_Implementation(FTransform HeadTransform, FTransform LeftTransform, FTransform RightTransform)
{
	VRCamera->SetRelativeTransform(HeadTransform);
	RightHand->SetRelativeTransform(RightTransform);
	LeftHand->SetRelativeTransform(LeftTransform);
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

void ABaseCharacter::ServerTrashSpawningPoolActivate_Implementation()
{
	TrashSpawningPool->IsActivate = true;
}

void ABaseCharacter::ServerSpawnThrowingWeapon_Implementation(FVector SpawnLoc, FRotator SpawnRot)
{
	AThrowingWeapon *ThrowingWeaponActor = GetWorld()->SpawnActor<AThrowingWeapon>(SpawnThrowingWeapon, SpawnLoc, SpawnRot);
	MulticastVRSoundPlay(FString("Bullet"));
}

void ABaseCharacter::ServerHealEffectActivate_Implementation(bool IsActivate)
{
	if (IsActivate)
	{
		LeftVRHealEffect->SetVisibility(true);
		RightVRHealEffect->SetVisibility(true);
	}
	else
	{
		LeftVRHealEffect->SetVisibility(false);
		RightVRHealEffect->SetVisibility(false);
	}
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
		DotEnd = HitInfo.Location;
		RedDot->SetActorLocation(DotEnd);
		Distance = HitInfo.Distance;
	}
	else
	{
		RedDot->SetActorLocation(DotEnd);
		Distance = (DotEnd - DotStart).Size();
	}
	TArray<FVector> BulletTraces;
	BulletTraces.Add(DotStart);
	BulletTraces.Add(DotEnd);
	BulletAimTraceComp->SetVisibility(true);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(BulletAimTraceComp, FName(TEXT("User.PointArray")), BulletTraces);
	RedDot->SetActorScale3D(FVector(Distance / 500));
	RedDot->SetActorRotation((-RedDot->GetActorLocation() + VRCamera->GetComponentLocation()).Rotation());
}

void ABaseCharacter::VRGetDamage(float Damage)
{
	ServerVRGetDamage(Damage);
	if (Damage < 0)
	{
		VRCamera->PostProcessSettings.WeightedBlendables.Array[1].Weight = 1;
	}
}

void ABaseCharacter::ServerVRGetDamage_Implementation(float Damage)
{
	VRCurHP -= Damage;
	VRCurHP = VRCurHP < 0 ? 0 : VRCurHP;
	VRCurHP = VRCurHP > VRMaxHP ? VRMaxHP : VRCurHP;
	float Rate = 1 - (VRCurHP / VRMaxHP);
	MulticastVRGetDamage(Rate);
	if (Damage > 0)
	{
		VRDamageEffectRate += 1;
		if (VRDamageEffectRate > 2)
		{
			VRDamageEffectRate = 2;
		}
	}
	
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

void ABaseCharacter::ServerVRAttack_Implementation(const FString &Position, class ABaseCharacter *Enemy)
{
	if (Position == FString("Sword"))
	{
		Enemy->OnTakeDamage(30);
		MulticastVRSoundPlay(FString("SwordSlash"));
		// Enemy->Server_TakeDamage(10);
	}
}

void ABaseCharacter::ServerVRReviveSetting_Implementation()
{
	VRHeadActor->SetActorLocation(FVector(0, 0, -1000));
	VRLeftHandActor->SetActorLocation(FVector(0, 0, -1000));
	VRRightHandActor->SetActorLocation(FVector(0, 0, -1000));
	VRHeadActor->MulticastHeadColorChange(0);
}

void ABaseCharacter::MulticastVRReviveSetting_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("REVIVE!!"));
	FTimerHandle VRReviveTimer;
	GetWorld()->GetTimerManager().SetTimer(VRReviveTimer,
										   FTimerDelegate::CreateLambda([this]() -> void
																		{ IsVRDead = false; }),
										   1, false);
	IsVRRevive = false;
	VRReviveTime = 0;
	ServerVRDeath(false);
	EnableInput(VRController);
	VRGetDamage(-VRMaxHP);
	VRCamera->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);
	VRCamera->PostProcessSettings.WeightedBlendables.Array[1].Weight = 0;
}

void ABaseCharacter::ServerVRDeath_Implementation(bool IsVRAlive)
{
	if (IsVRAlive)
	{
		VRHeadActor->SetActorLocationAndRotation(VRHeadMesh->GetComponentLocation(), VRHeadMesh->GetComponentRotation());
		VRLeftHandActor->SetActorLocationAndRotation(LeftHandMesh->GetComponentLocation(), LeftHandMesh->GetComponentRotation());
		VRRightHandActor->SetActorLocationAndRotation(RightHandMesh->GetComponentLocation(), RightHandMesh->GetComponentRotation());
		FTimerHandle VRDeathTimer1;
		GetWorld()->GetTimerManager().SetTimer(VRDeathTimer1,
			FTimerDelegate::CreateLambda([this]() -> void
				{
					VRHeadActor->MeshComp->SetSimulatePhysics(true);
					VRLeftHandActor->MeshComp->SetSimulatePhysics(true);
					VRRightHandActor->MeshComp->SetSimulatePhysics(true);
					SetActorLocation(FVector(200, -100, 300));
					//AddControllerYawInput(90 - GetActorRotation().Yaw);
				}),
											   0.5f, false);
		FTimerHandle VRDeathTimer2;
		GetWorld()->GetTimerManager().SetTimer(VRDeathTimer2,
											   FTimerDelegate::CreateLambda([this]() -> void
																			{
					VRHeadActor->MeshComp->SetSimulatePhysics(false);
					VRLeftHandActor->MeshComp->SetSimulatePhysics(false);
					VRRightHandActor->MeshComp->SetSimulatePhysics(false); }),
											   1.9f, false);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	else
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		MulticastVRSetting();
	}
}

void ABaseCharacter::ServerVRRevive_Implementation(float Rate, FVector HeadLoc, FVector LHandLoc, FVector RHandLoc, FRotator HeadRot, FRotator LHandRot, FRotator RHandRot)
{
	VRHeadActor->SetActorLocation(HeadLoc);
	VRLeftHandActor->SetActorLocation(LHandLoc);
	VRRightHandActor->SetActorLocation(RHandLoc);

	VRHeadActor->SetActorRotation(HeadRot);
	VRLeftHandActor->SetActorRotation(LHandRot);
	VRRightHandActor->SetActorRotation(RHandRot);

	MulticastVRRevive(Rate);
}

void ABaseCharacter::MulticastVRRevive_Implementation(float Rate)
{
	VRHeadActor->MulticastHeadColorChange(Rate);
}

bool ABaseCharacter::VRSkillCheck(FString Position)
{
	if (Position == FString("Left"))
	{
		if (!IsLeftY && !IsLeftX && !IsLeftGrab)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (Position == FString("Right"))
	{
		if (!IsRightA && !IsRightB && !IsRightGrab)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (Position == FString("Hands"))
	{
		if (!IsLeftY && !IsLeftX && !IsLeftGrab && !IsRightA && !IsRightB && !IsRightGrab && !IsVRDead && !IsVRRevive)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void ABaseCharacter::ServerVRDoorBreak_Implementation(const FString &Position, UPrimitiveComponent *OtherComp)
{
	if (Position == FString("Left"))
	{
		GetWorld()->SpawnActor<AActor>(BreakDoor, LeftHand->GetComponentLocation(), LeftHand->GetComponentRotation());
		LeftWidgetInteraction->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OtherComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		BreakableDoor = OtherComp->GetOwner();
		BreakableDoor->SetActorEnableCollision(false);
		FTimerHandle DoorDestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(DoorDestroyTimer,
											   FTimerDelegate::CreateLambda([this]() -> void
																			{
					if (BreakableDoor)
					{
						BreakableDoor->Destroy();
					} }),
											   5, false);
	}
	else if (Position == FString("Right"))
	{
		GetWorld()->SpawnActor<AActor>(BreakDoor, RightHand->GetComponentLocation(), RightHand->GetComponentRotation());
		RightWidgetInteraction->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OtherComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		BreakableDoor = OtherComp->GetOwner();
		BreakableDoor->SetActorEnableCollision(false);
		FTimerHandle DoorDestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(DoorDestroyTimer,
											   FTimerDelegate::CreateLambda([this]() -> void
																			{
					if (BreakableDoor)
					{
						BreakableDoor->Destroy();
					} }),
											   5, false);
	}

	MulticastVRDoorBreak();
}

void ABaseCharacter::MulticastVRDoorBreak_Implementation()
{
	if (BridgeState->GetGameState() == EGameState::Wait)
	{
		BridgeState->SetGameState(EGameState::Start);
	}
}

void ABaseCharacter::ServerVRSoundPlay_Implementation(const FString& Position)
{
	if (Position == FString("SwordCast"))
	{
		MulticastVRSoundPlay(FString("SwordCast"));
	}
}

void ABaseCharacter::MulticastVRSoundPlay_Implementation(const FString& Position)
{
	if (Position == FString("Bullet"))
	{
		UGameplayStatics::PlaySoundAtLocation(this, VRFireSound, RightAim->GetComponentLocation());
	}
	else if (Position == FString("SwordCast"))
	{
		SwordCastSound = UGameplayStatics::SpawnSoundAtLocation(this, VRSwordCastSound, SwordMesh->GetComponentLocation());
	}
	else if (Position == FString("SwordCastStop"))
	{
		if (SwordCastSound)
		{
			SwordCastSound->Stop();
		}
	}
	else if (Position == FString("SwordSlash"))
	{
		UGameplayStatics::PlaySoundAtLocation(this, VRSwordSlashSound, SwordMesh->GetComponentLocation());
	}
}