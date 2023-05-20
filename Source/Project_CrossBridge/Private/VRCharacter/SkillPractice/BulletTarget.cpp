// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/BulletTarget.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects/ThrowingWeapon.h"
#include "GeometryCollection/GeometryCollectionComponent.h"


// Sets default values
ABulletTarget::ABulletTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(false);
	SetRootComponent(MeshComp);
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(MeshComp);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletTarget::OnOverlap);
}

// Called when the game starts or when spawned
void ABulletTarget::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsHorizontal)
	{
		StartLoc = GetActorLocation();
		ReturnPoint1 = StartLoc + GetActorRightVector() * 400;
		ReturnPoint2 = StartLoc - GetActorRightVector() * 400;
		SkillRate = 5;
	}
	else if (IsVertical)
	{
		StartLoc = GetActorLocation();
		ReturnPoint1 = StartLoc + GetActorUpVector() * 400;
		ReturnPoint2 = StartLoc - GetActorUpVector() * 400;
		SkillRate = 5;
	}
	else if (IsWhirl)
	{
		StartLoc = GetActorLocation();
		SkillRate = 1;
	}
}

// Called every frame
void ABulletTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rate += DeltaTime / SkillRate;

	if (IsHorizontal)
	{
		if (Rate <= 1)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(StartLoc, ReturnPoint1, Rate));
		}
		else if (Rate <= 2)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(ReturnPoint1, StartLoc, Rate - 1));
		}
		else if (Rate <= 3)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(StartLoc, ReturnPoint2, Rate - 2));
		}
		else if (Rate <= 4)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(ReturnPoint2, StartLoc, Rate - 3));
		}
		else
		{
			Rate = 0;
		}
	}
	else if (IsVertical)
	{
		if (Rate <= 1)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(StartLoc, ReturnPoint1, Rate));
		}
		else if (Rate <= 2)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(ReturnPoint1, StartLoc, Rate - 1));
		}
		else if (Rate <= 3)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(StartLoc, ReturnPoint2, Rate - 2));
		}
		else if (Rate <= 4)
		{
			SetActorLocation(UKismetMathLibrary::VLerp(ReturnPoint2, StartLoc, Rate - 3));
		}
		else
		{
			Rate = 0;
		}
	}
	else if (IsWhirl)
	{
		float ReturnPoint = 15;
		if (Rate > 0 && Rate < ReturnPoint)
		{
			SetActorLocation(StartLoc +  15 * FVector(Rate * UKismetMathLibrary::Cos(Rate), Rate * UKismetMathLibrary::Sin(Rate), Rate * 1.5f));
		}
		else if (Rate < ReturnPoint * 2)
		{
			float Rev = ReturnPoint * 2 - Rate;
			SetActorLocation(StartLoc + 15 *  FVector(Rev * UKismetMathLibrary::Cos(Rev), Rev * UKismetMathLibrary::Sin(Rev), Rev * 1.5f));
		}
		else
		{
			Rate = 0;
		}
	}
}

void ABulletTarget::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AThrowingWeapon>(OtherActor))
	{
		MeshComp->SetSimulatePhysics(true);
		SpawnPoint = OtherActor->GetActorLocation();
		GetWorld()->SpawnActor<AActor>(BreakActor, SpawnPoint, GetActorRotation());
		FTimerHandle DestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer,
			FTimerDelegate::CreateLambda([this]()->void
				{
					Destroy();
				}), 3, false);
	}
}
