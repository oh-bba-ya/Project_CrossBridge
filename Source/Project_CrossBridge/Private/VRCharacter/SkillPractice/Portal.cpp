// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/SkillPractice/Portal.h"
#include "Components/BoxComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "VRCharacter/SkillPractice/Widget/SkillTestCompleteWidget.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StartPortalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartPortalBox"));
	StartPortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartPortalMesh"));
	StartPortalMesh->SetupAttachment(StartPortalBox);

	EndPortalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EndPortalBox"));
	EndPortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndPortalMesh"));
	EndPortalMesh->SetupAttachment(EndPortalBox);

	SuccessWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SuccessWidget"));
	SuccessWidget->SetupAttachment(EndPortalBox);

	StartPortalBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnStartPortalOverlap);
	EndPortalBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnEndPortalOverlap);

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::OnStartPortalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	class ABaseCharacter* VRUser = Cast<ABaseCharacter>(OtherActor);
	if (VRUser)
	{
		VRUser->SetActorLocation(EndPortalBox->GetComponentLocation());
	}

}
void APortal::OnEndPortalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	class ABaseCharacter* VRUser = Cast<ABaseCharacter>(OtherActor);
	if (VRUser)
	{
		if (Cast<USkillTestCompleteWidget>(SuccessWidget->GetWidget())->Index >= 8)
		{
			StartPortalBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			VRUser->SetActorLocation(StartPortalBox->GetComponentLocation());
		}
	}
}
