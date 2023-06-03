// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/PracticeActorSpawn.h"
#include "VRCharacter/SkillPractice/BulletTarget.h"
#include "VRCharacter/SkillPractice/SwordTarget.h"
#include "Components/BoxComponent.h"
#include "Character/BaseCharacter.h"
#include "EngineUtils.h"
#include "Objects/Trash.h"
#include "Components/WidgetComponent.h"
#include "VRCharacter/SkillPractice/Widget/BulletTestWidget.h"
#include "VRCharacter/SkillPractice/Widget/BlackholeTestWidget.h"
#include "VRCharacter/SkillPractice/Widget/TrashTestWidget.h"
#include "VRCharacter/SkillPractice/Widget/SwordTestWidget.h"
#include "VRCharacter/SkillPractice/Widget/HealTestWidget.h"
#include "VRCharacter/SkillPractice/Widget/SkillTestCompleteWidget.h"
#include "VRCharacter/SkillPractice/Portal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APracticeActorSpawn::APracticeActorSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletTestWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BulletTestWidget"));
	BulletTestWidget->SetVisibility(false);
	BlackholeCheckComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BlackholeCheckComp"));
	BlackholeCheckComp->SetupAttachment(RootComponent);
	BlackholeTestWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BlackholeTestWidget"));
	BlackholeTestWidget->SetVisibility(false);

	TrashCheckComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TrashCheckComp"));
	TrashCheckComp->SetupAttachment(RootComponent);
	TrashTestWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TrashTestWidget"));
	TrashTestWidget->SetVisibility(false);

	SwordTestWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SwordTestWidget"));
	SwordTestWidget->SetVisibility(false);

	HealTestComp = CreateDefaultSubobject<UBoxComponent>(TEXT("HealTestComp"));
	HealTestComp->SetupAttachment(RootComponent);
	HealTestWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealTestWidget"));
	HealTestWidget->SetVisibility(false);

}

// Called when the game starts or when spawned
void APracticeActorSpawn::BeginPlay()
{
	Super::BeginPlay();

	SuccessCheckActor = Cast<APortal>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal::StaticClass()));
	if (SuccessCheckActor)
	{
		//SuccessCheck = Cast<USkillTestCompleteWidget>(SuccessCheckActor->SuccessWidget->GetWidget());
		FTimerHandle WidgetCastTimer;
		GetWorld()->GetTimerManager().SetTimer(WidgetCastTimer,
			FTimerDelegate::CreateLambda([this]()->void {
				SuccessCheck = Cast<USkillTestCompleteWidget>(SuccessCheckActor->SuccessWidget->GetWidget());
				}), 1, false);
	}
	
	if (IsBulletTest)
	{
		BulletTarget = GetWorld()->SpawnActor<ABulletTarget>(SpawnBulletTarget1, GetActorLocation(), GetActorRotation());
		BulletTestWidget->SetVisibility(true);
		BulletPattern++;
	}
	else if (IsBlackholeTest)
	{
		BlackholeTestWidget->SetVisibility(true);
		BlackholeCheckComp->OnComponentBeginOverlap.AddDynamic(this, &APracticeActorSpawn::OnBlackholeOverlap);
		for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
		{
			if (It->IsDummy && !It->IsSwordTestDummy)
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
	else if (IsTrashTest)
	{
		TrashTestWidget->SetVisibility(true);
		TrashCheckComp->OnComponentBeginOverlap.AddDynamic(this, &APracticeActorSpawn::OnTrashOverlap);
		TrashCheckComp->SetCollisionProfileName(TEXT("PlayerPreset"));
	}
	else if (IsSwordTest)
	{
		SwordTestWidget->SetVisibility(true);
		SwordTarget = GetWorld()->SpawnActor<ASwordTarget>(SpawnSwordTarget, GetActorLocation(), GetActorRotation());
	}
	else if (IsHealTest)
	{
		HealTestComp->OnComponentBeginOverlap.AddDynamic(this, &APracticeActorSpawn::OnHealOverlap);
		HealTestComp->SetCollisionProfileName(TEXT("PlayerPreset"));
		HealTestWidget->SetVisibility(true);

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
	if (IsBulletTest && (!BulletTarget || BulletTarget->IsPendingKill()))
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
			Cast<UBulletTestWidget>(BulletTestWidget->GetWidget())->SetImageVisibility(1);
			SuccessCheck->Index++;
			SuccessCheck->SetImageVisibility(SuccessCheck->Index);
		}
		else if (BulletPattern == 2)
		{
			BulletTarget = GetWorld()->SpawnActor<ABulletTarget>(SpawnBulletTarget3, GetActorLocation(), GetActorRotation());
			BulletPattern++;
			Cast<UBulletTestWidget>(BulletTestWidget->GetWidget())->SetImageVisibility(2);
			SuccessCheck->Index++;
			SuccessCheck->SetImageVisibility(SuccessCheck->Index);
		}
		else if (BulletPattern == 3)
		{
			BulletPattern++;
			Cast<UBulletTestWidget>(BulletTestWidget->GetWidget())->SetImageVisibility(3);
			FTimerHandle SuccessImageTimer;
			GetWorld()->GetTimerManager().SetTimer(SuccessImageTimer,
				FTimerDelegate::CreateLambda([this]()->void {
					Cast<UBulletTestWidget>(BulletTestWidget->GetWidget())->SetImageVisibility(4);
					}), 1, false);
			SuccessCheck->Index++;
			SuccessCheck->SetImageVisibility(SuccessCheck->Index);
			//BulletPattern = 0;
		}
	}
	else if (IsSwordTest && (!SwordTarget || SwordTarget->IsPendingKill()))
	{
		IsSwordTest = false;
		Cast<USwordTestWidget>(SwordTestWidget->GetWidget())->SetImageVisibility(1);
		FTimerHandle SuccessImageTimer;
		GetWorld()->GetTimerManager().SetTimer(SuccessImageTimer,
			FTimerDelegate::CreateLambda([this]()->void {
				Cast<USwordTestWidget>(SwordTestWidget->GetWidget())->SetImageVisibility(2);
				}), 1, false);
		SuccessCheck->Index++;
		SuccessCheck->SetImageVisibility(SuccessCheck->Index);
	}
}

void APracticeActorSpawn::OnBlackholeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TestActor1 == Cast<ABaseCharacter>(OtherActor))
	{
		IsTestActor1 = true;
	}
	else if (TestActor2 == Cast<ABaseCharacter>(OtherActor))
	{
		IsTestActor2 = true;
	}

	if (IsTestActor1 && IsTestActor2)
	{
		//TestActor1->SetActorLocation(TestActorLoc1);
		//TestActor2->SetActorLocation(TestActorLoc2);
		IsTestActor1 = false;
		IsTestActor2 = false;
		Cast<UBlackholeTestWidget>(BlackholeTestWidget->GetWidget())->SetImageVisibility(1);
		FTimerHandle SuccessImageTimer;
		GetWorld()->GetTimerManager().SetTimer(SuccessImageTimer,
			FTimerDelegate::CreateLambda([this]()->void {
				Cast<UBlackholeTestWidget>(BlackholeTestWidget->GetWidget())->SetImageVisibility(2);
				}), 1, false);
		SuccessCheck->Index++;
		SuccessCheck->SetImageVisibility(SuccessCheck->Index);
	}

}

void APracticeActorSpawn::OnTrashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATrash>(OtherActor))
	{
		TrashCount++;
		if (TrashCount <= 20)
		{
			Cast<UTrashTestWidget>(TrashTestWidget->GetWidget())->SetTrashCountText(TrashCount);
			if (TrashCount == 20)
			{
				Cast<UTrashTestWidget>(TrashTestWidget->GetWidget())->SetImageVisibility(1);
				FTimerHandle SuccessImageTimer;
				GetWorld()->GetTimerManager().SetTimer(SuccessImageTimer,
					FTimerDelegate::CreateLambda([this]()->void {
						Cast<UTrashTestWidget>(TrashTestWidget->GetWidget())->SetImageVisibility(2);
						}), 1, false);
				SuccessCheck->Index++;
				SuccessCheck->SetImageVisibility(SuccessCheck->Index);
			}
		}
	}
}

void APracticeActorSpawn::OnHealOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel3)
	{
		class ABaseCharacter* VRUser = Cast<ABaseCharacter>(OtherActor);
		if (!IsCheck)
		{
			if (!IsHit)
			{
				Cast<UHealTestWidget>(HealTestWidget->GetWidget())->SetImageVisibility(1);
				SuccessCheck->Index++;
				SuccessCheck->SetImageVisibility(SuccessCheck->Index);
				VRUser->VRGetDamage(80);
				IsHit = true;
			}
			else if (IsHit && VRUser->VRCurHP == VRUser->VRMaxHP)
			{
				Cast<UHealTestWidget>(HealTestWidget->GetWidget())->SetImageVisibility(2);
				FTimerHandle SuccessImageTimer;
				GetWorld()->GetTimerManager().SetTimer(SuccessImageTimer,
					FTimerDelegate::CreateLambda([this]()->void {
						Cast<UHealTestWidget>(HealTestWidget->GetWidget())->SetImageVisibility(3);
						}), 1, false);
				SuccessCheck->Index++;
				SuccessCheck->SetImageVisibility(SuccessCheck->Index);
				IsCheck = true;
			}
		}
	}

}