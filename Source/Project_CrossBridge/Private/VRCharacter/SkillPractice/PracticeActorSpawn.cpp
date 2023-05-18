// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/PracticeActorSpawn.h"
#include "VRCharacter/SkillPractice/BulletTarget.h"
#include "Components/BoxComponent.h"
#include "Character/BaseCharacter.h"
#include "EngineUtils.h"

// Sets default values
APracticeActorSpawn::APracticeActorSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlackholeCheckComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BlackholeCheckComp"));
}

// Called when the game starts or when spawned
void APracticeActorSpawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsBulletTest)
	{
		BulletTarget = GetWorld()->SpawnActor<ABulletTarget>(SpawnBulletTarget1, GetActorLocation(), GetActorRotation());
		BulletPattern++;
	}
	else if (IsBlackholeTest)
	{
		BlackholeCheckComp->OnComponentBeginOverlap.AddDynamic(this, &APracticeActorSpawn::OnOverlap);
		for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
		{
			if (It->IsDummy)
			{
				if (!TestActor1)
				{
					TestActor1 = *It;
				}
				else if (!TestActor2)
				{
					TestActor2 = *It;
				}
				if (TestActor1 && TestActor2)
				{
					TestActorLoc1 = TestActor1->GetActorLocation();
					TestActorLoc2 = TestActor2->GetActorLocation();
					break;
				}
			}
		}
	}
}

// Called every frame
void APracticeActorSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnObject();
}

void APracticeActorSpawn::SpawnObject()
{
	if (IsBulletTest && BulletTarget->IsPendingKill())
	{
		if (BulletPattern == 0)
		{
			BulletTarget = GetWorld()->SpawnActor<ABulletTarget>(SpawnBulletTarget1, GetActorLocation(), GetActorRotation());
			BulletPattern++;
		}
		else if (BulletPattern == 1)
		{
			BulletTarget = GetWorld()->SpawnActor<ABulletTarget>(SpawnBulletTarget2, GetActorLocation(), GetActorRotation());
			BulletPattern++;
		}
		else if (BulletPattern == 2)
		{
			BulletTarget = GetWorld()->SpawnActor<ABulletTarget>(SpawnBulletTarget3, GetActorLocation(), GetActorRotation());
			BulletPattern++;
		}
		else
		{
			BulletPattern = 0;
		}
	}
}

void APracticeActorSpawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TestActor1 == Cast<ABaseCharacter>(OtherActor))
	{
		IsTestActor1 = true;
	}
	else if (TestActor2 == Cast<ABaseCharacter>(OtherActor))
	{
		IsTestActor2 = true;
	}

	if (TestActor1 && TestActor2)
	{
		TestActor1->SetActorLocation(TestActorLoc1);
		TestActor2->SetActorLocation(TestActorLoc2);
		IsTestActor1 = false;
		IsTestActor2 = false;
	}

}