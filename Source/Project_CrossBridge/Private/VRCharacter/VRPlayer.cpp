// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/VRPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Objects/BaseGrabbableActor.h"
#include "Kismet/KismetMathLibrary.h"


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

	LeftGrip = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftGrip"));
	LeftGrip->SetupAttachment(RootComponent);
	LeftGrip->SetTrackingMotionSource(FName("LeftGrip"));

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

	LeftHandBox->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayer::OnLeftHandOverlap);
	LeftHandBox->OnComponentEndOverlap.AddDynamic(this, &AVRPlayer::OnLeftHandEndOverlap);
	RightHandBox->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayer::OnRightHandOverlap);
	RightHandBox->OnComponentEndOverlap.AddDynamic(this, &AVRPlayer::OnRightHandEndOverlap);

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
			//UE_LOG(LogTemp, Warning, TEXT("AS"));
			SubSystem->AddMappingContext(IMC_VRInput, 0);
			SubSystem->AddMappingContext(IMC_VRHand, 0);
		}
	}

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
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
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetGrabInfo();

	if (IsLeftY)
	{
		LeftYTimer += DeltaTime;
		if (LeftYTimer / 5 <= 1)
		{

			//ColorChange(LeftYTimer / 5, FString("Left"));
			ServerColorChange(LeftYTimer / 5, FString("Left"));
		}

	}
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
		InputSystem->BindAction(IA_LeftIndexCurl, ETriggerEvent::Triggered, this, &AVRPlayer::LeftIndexCurl);
		InputSystem->BindAction(IA_LeftGrasp, ETriggerEvent::Triggered, this, &AVRPlayer::LeftGrasp);
		InputSystem->BindAction(IA_LeftY, ETriggerEvent::Triggered, this, &AVRPlayer::LeftY);
		InputSystem->BindAction(IA_RightIndexCurl, ETriggerEvent::Triggered, this, &AVRPlayer::RightIndexCurl);
		InputSystem->BindAction(IA_RightGrasp, ETriggerEvent::Triggered, this, &AVRPlayer::RightGrasp);	
		InputSystem->BindAction(IA_LeftIndexCurl, ETriggerEvent::Completed, this, &AVRPlayer::LeftIndexCurlEnd);
		InputSystem->BindAction(IA_LeftGrasp, ETriggerEvent::Completed, this, &AVRPlayer::LeftGraspEnd);
		InputSystem->BindAction(IA_LeftY, ETriggerEvent::Completed, this, &AVRPlayer::LeftYEnd);
		InputSystem->BindAction(IA_RightIndexCurl, ETriggerEvent::Completed, this, &AVRPlayer::RightIndexCurlEnd);
		InputSystem->BindAction(IA_RightGrasp, ETriggerEvent::Completed, this, &AVRPlayer::RightGraspEnd);
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

void AVRPlayer::LeftIndexCurl()
{
	IsLeftIndexCurl = true;
	if (GrabbedActorLeft && IsLeftGrasp && !IsLeftGrab)
	{
		GrabTheActor(GrabbedActorLeft, FString("Left"));
	}
}

void AVRPlayer::LeftGrasp()
{
	IsLeftGrasp = true;
	if (GrabbedActorLeft && IsLeftIndexCurl && !IsLeftGrab)
	{
		GrabTheActor(GrabbedActorLeft, FString("Left"));
	}
}

void AVRPlayer::LeftY()
{
	if (!IsLeftIndexCurl && !IsLeftGrasp)
	{
		IsLeftY = true;
	}
}
void AVRPlayer::LeftYEnd()
{
	if (LeftYTimer > 1)
	{
		ServerSpawnGrabbableActor();
	}
	IsLeftY = false;
	LeftYTimer = 0;
	//ResetColorChange(FString("Left"));
	ServerResetColorChange(FString("Left"));
}

void AVRPlayer::RightIndexCurl()
{
	IsRightIndexCurl = true;
	if (GrabbedActorRight && IsRightGrasp && !IsRightGrab)
	{
		GrabTheActor(GrabbedActorRight, FString("Right"));
		RightPrevLoc = RightHand->GetComponentLocation();
		RightPrevRot = RightHand->GetComponentQuat();
	}
}

void AVRPlayer::RightGrasp()
{
	IsRightGrasp = true;
	if (GrabbedActorRight && IsRightIndexCurl && !IsRightGrab)
	{
		GrabTheActor(GrabbedActorRight, FString("Right"));
		RightPrevLoc = RightHand->GetComponentLocation();
		RightPrevRot = RightHand->GetComponentQuat();
	}

}
void AVRPlayer::LeftIndexCurlEnd()
{
	IsLeftIndexCurl = false;
	if (IsLeftGrab)
	{
		UnGrabTheActor(GrabbedActorLeft, FString("Left"));
	}
}

void AVRPlayer::LeftGraspEnd()
{
	IsLeftGrasp = false;
	if (IsLeftGrab)
	{
		UnGrabTheActor(GrabbedActorLeft, FString("Left"));
	}
}

void AVRPlayer::RightIndexCurlEnd()
{
	IsRightIndexCurl = false;
	if (IsRightGrab)
	{
		UnGrabTheActor(GrabbedActorRight, FString("Right"));

	}
}

void AVRPlayer::RightGraspEnd()
{
	IsRightGrasp = false;
	if (IsRightGrab)
	{
		UnGrabTheActor(GrabbedActorRight, FString("Right"));

	}

}

void AVRPlayer::OnLeftHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GrabbedActorLeft)
	{
		GrabbedActorLeft = Cast<ABaseGrabbableActor>(OtherActor);
	}

}

void AVRPlayer::OnLeftHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (GrabbedActorLeft && GrabbedActorLeft == Cast<ABaseGrabbableActor>(OtherActor))
	{	
		GrabbedActorLeft = NULL;
	}

}

void AVRPlayer::OnRightHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GrabbedActorRight)
	{
		GrabbedActorRight = Cast<ABaseGrabbableActor>(OtherActor);
	}

}

void AVRPlayer::OnRightHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GrabbedActorRight && GrabbedActorRight == Cast<ABaseGrabbableActor>(OtherActor))
	{
		GrabbedActorRight = NULL;
	}

}

void AVRPlayer::GrabTheActor(ABaseGrabbableActor* GrabbedActor, FString GrabPosition)
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

void AVRPlayer::UnGrabTheActor(ABaseGrabbableActor* GrabbedActor, FString GrabPosition)
{
	GrabbedActor->MeshComp->SetSimulatePhysics(true);
	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (GrabPosition == FString("Left"))
	{
		IsLeftGrab = false;
		GrabbedActorLeft = NULL;
	}
	else if (GrabPosition == FString("Right"))
	{
		IsRightGrab = false;
		GrabbedActor->MeshComp->AddForce(ThrowPower * RightThrowDir);

		float Angle;
		FVector Axis;
		float dt = GetWorld()->DeltaTimeSeconds;
		RightThrowRot.ToAxisAndAngle(Axis, Angle);
		FVector AngularVelocity = (1.0f / dt) * Angle * Axis;
		GrabbedActor->MeshComp->SetPhysicsAngularVelocityInRadians(AngularVelocity * ToquePower, true);
		
		GrabbedActorRight = NULL;
	}

}

void AVRPlayer::SetGrabInfo()
{
	if (IsRightGrab)
	{
		RightThrowDir = RightHand->GetComponentLocation() - RightPrevLoc;
		RightThrowRot = RightHand->GetComponentQuat() * RightPrevRot.Inverse();

		RightPrevLoc = RightHand->GetComponentLocation();
		RightPrevRot = RightHand->GetComponentQuat();
	}
}

void AVRPlayer::ColorChange(float Rate, FString Position)
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

void AVRPlayer::ServerColorChange_Implementation(float Rate, const FString& Position)
{
	MulticastColorChange(Rate, Position);
}
void AVRPlayer::MulticastColorChange_Implementation(float Rate, const FString& Position)
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

void AVRPlayer::ResetColorChange(FString Position)
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

void AVRPlayer::ServerResetColorChange_Implementation(const FString& Position)
{
	MulticastResetColorChange(Position);
}


void AVRPlayer::MulticastResetColorChange_Implementation(const FString& Position)
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
void AVRPlayer::ServerSpawnGrabbableActor_Implementation()
{
	ABaseGrabbableActor* GrabActor = GetWorld()->SpawnActor<ABaseGrabbableActor>(SpawnGrabbedActor, LeftHand->GetComponentLocation(), LeftHand->GetComponentRotation());
}