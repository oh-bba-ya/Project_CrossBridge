// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/VRPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AVRPlayer::AVRPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;

	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));

	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));

	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	RightAim->SetupAttachment(RootComponent);
	RightAim->SetTrackingMotionSource(FName("RightAim"));
   
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
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

}

// Called when the game starts or when spawned
void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		auto LocalPlayer = PC->GetLocalPlayer();
		auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (SubSystem)
		{
			SubSystem->AddMappingContext(IMC_VRInput, 0);
			SubSystem->AddMappingContext(IMC_VRHand, 0);
		}
	}

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	}

}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputSystem)
	{
		InputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AVRPlayer::Move);
		InputSystem->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AVRPlayer::Turn);
	}

}

void AVRPlayer::Move(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);


}

void AVRPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);

}
