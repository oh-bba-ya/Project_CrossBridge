// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Blackhole.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/BaseCharacter.h"
#include "NiagaraComponent.h"


// Sets default values
ABlackhole::ABlackhole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(300);
	SetRootComponent(SphereComp);

	BlackSphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackSphereMeshComp"));
	BlackSphereMeshComp->SetRelativeRotation(FRotator(14, 0, 0));
	BlackSphereMeshComp->SetRelativeScale3D(FVector(0.875));
	BlackSphereMeshComp->SetupAttachment(RootComponent);

	ShineSphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShineSphereMeshComp"));
	ShineSphereMeshComp->SetRelativeRotation(FRotator(-14, 180, 176));
	ShineSphereMeshComp->SetRelativeScale3D(FVector(1.125));
	ShineSphereMeshComp->SetupAttachment(RootComponent);

	FloorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMeshComp"));
	FloorMeshComp->SetRelativeLocation(FVector(0, 0, 1));
	FloorMeshComp->SetRelativeRotation(FRotator(0, 180, 0));
	FloorMeshComp->SetRelativeScale3D(FVector(6.625, 6.625, 0.125));
	FloorMeshComp->SetupAttachment(RootComponent);
	
	CenterNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CenterNiagara"));
	CenterNiagaraComp->SetupAttachment(RootComponent);
	CenterNiagaraComp->SetVisibility(false);
	ExternalNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExternalNiagara"));
	ExternalNiagaraComp->SetupAttachment(RootComponent);
	ExternalNiagaraComp->SetVisibility(false);

	ConstructorHelpers::FObjectFinder<UStaticMesh> BaseSphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/VRPlayer/MAT_Blackhole/Sphere.Sphere'"));
	if (BaseSphereMesh.Succeeded())
	{
		BlackSphereMeshComp->SetStaticMesh(BaseSphereMesh.Object);
		ShineSphereMeshComp->SetStaticMesh(BaseSphereMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> BasePlaneMesh(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/VRPlayer/MAT_Blackhole/Plane.Plane'"));
	if (BasePlaneMesh.Succeeded())
	{
		FloorMeshComp->SetStaticMesh(BasePlaneMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> TempCenterMat(TEXT("/Script/Engine.Material'/Game/Blueprints/VRPlayer/MAT_Blackhole/M_Blackhole_B_Center.M_Blackhole_B_Center'"));
	if (TempCenterMat.Succeeded())
	{
		BlackSphereMeshComp->SetMaterial(0, TempCenterMat.Object);
		BlackSphereMeshComp->SetMaterial(2, TempCenterMat.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> TempShineSphereMat(TEXT("/Script/Engine.Material'/Game/Blueprints/VRPlayer/MAT_Blackhole/M_Blackhole_B.M_Blackhole_B'"));
	if (TempShineSphereMat.Succeeded())
	{
		ShineSphereMeshComp->SetMaterial(0, TempShineSphereMat.Object);
		ShineSphereMeshComp->SetMaterial(2, TempShineSphereMat.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> TempFloorMat(TEXT("/Script/Engine.Material'/Game/Blueprints/VRPlayer/MAT_Blackhole/M_Blackhole_B_Background.M_Blackhole_B_Background'"));
	if (TempFloorMat.Succeeded())
	{
		FloorMeshComp->SetMaterial(0, TempFloorMat.Object);
		FloorMeshComp->SetMaterial(2, TempFloorMat.Object);
	}
	
	ConstructorHelpers::FObjectFinder<UMaterial> CenterMat(TEXT("/Script/Engine.Material'/Game/Blueprints/VRPlayer/MAT_Blackhole/M_BlackBackground_D.M_BlackBackground_D'"));
	if (CenterMat.Succeeded())
	{
		BlackSphereMeshComp->SetMaterial(1, CenterMat.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> ShineSphereMat(TEXT("/Script/Engine.Material'/Game/Blueprints/VRPlayer/MAT_Blackhole/M_BlackHole_D.M_BlackHole_D'"));
	if (ShineSphereMat.Succeeded())
	{
		ShineSphereMeshComp->SetMaterial(1, ShineSphereMat.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> FloorMat(TEXT("/Script/Engine.Material'/Game/Blueprints/VRPlayer/MAT_Blackhole/MT_DeadLine_D.MT_DeadLine_D'"));
	if (FloorMat.Succeeded())
	{
		FloorMeshComp->SetMaterial(1, FloorMat.Object);
	}

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackhole::OnOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ABlackhole::OnEndOverlap);
}

// Called when the game starts or when spawned
void ABlackhole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsBlackholeActive)
	{
		if (IsTarget1)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("Overlapped1111"));
			ServerBlackholeActive(1);
		}
		if (IsTarget2)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("Overlapped2222"));
			ServerBlackholeActive(2);
		}
	}
}

void ABlackhole::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Target = Cast<ABaseCharacter>(OtherActor);
	if (HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("Overlapped"));
		if (!Target1)
		{
			Target1 = Cast<ABaseCharacter>(OtherActor);
		}
		else if (!Target2)
		{
			Target2 = Cast<ABaseCharacter>(OtherActor);
		}
		if (Target1 == Cast<ABaseCharacter>(OtherActor))
		{
			IsTarget1 = true;
		}
		else if (Target2 == Cast<ABaseCharacter>(OtherActor))
		{
			IsTarget2 = true;
		}
	}
	

}
void ABlackhole::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		if (Target1 == Cast<ABaseCharacter>(OtherActor))
		{
			IsTarget1 = false;
		}
		else if (Target2 == Cast<ABaseCharacter>(OtherActor))
		{
			IsTarget2 = false;
		}
	}
}

void ABlackhole::ServerBlackholeActive_Implementation(int32 Num)
{
	if (Num == 1)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("111111"));
		FVector Dir = (GetActorLocation() - Target1->GetActorLocation()).GetSafeNormal();
		float Dist = GetDistanceTo(Target1);
		//Target1->AddMovementInput(Power * Dir / Dist);
		MulticastBlackholeActive(Target1, Power * Dir / Dist);
		
	}
	else if (Num == 2)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString("22222"));
		FVector Dir = (GetActorLocation() - Target2->GetActorLocation()).GetSafeNormal();
		float Dist = GetDistanceTo(Target2);
		//Target2->AddMovementInput(Power * Dir / Dist);
		MulticastBlackholeActive(Target2, Power * Dir / Dist);
	}

}
void ABlackhole::MulticastBlackholeActive_Implementation(class ABaseCharacter* Target, FVector Input)
{
	Target->AddMovementInput(Input);
}

void ABlackhole::BlackholeActiveSetting()
{
	MulticastBlackholeActiveSetting();
}

void ABlackhole::MulticastBlackholeActiveSetting_Implementation()
{
	BlackSphereMeshComp->SetMaterial(0, BlackSphereMeshComp->GetMaterial(1));
	ShineSphereMeshComp->SetMaterial(0, ShineSphereMeshComp->GetMaterial(1));
	FloorMeshComp->SetMaterial(0, FloorMeshComp->GetMaterial(1));
	CenterNiagaraComp->SetVisibility(true);
	ExternalNiagaraComp->SetVisibility(true);

}

void ABlackhole::BlackholeDeactivate()
{
	MulticastBlackholeDeactivate();
}
void ABlackhole::MulticastBlackholeDeactivate_Implementation()
{

	BlackSphereMeshComp->SetMaterial(0, BlackSphereMeshComp->GetMaterial(2));
	ShineSphereMeshComp->SetMaterial(0, ShineSphereMeshComp->GetMaterial(2));
	FloorMeshComp->SetMaterial(0, FloorMeshComp->GetMaterial(2));
	CenterNiagaraComp->SetVisibility(false);
	ExternalNiagaraComp->SetVisibility(false);

}