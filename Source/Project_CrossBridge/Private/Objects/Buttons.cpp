// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Buttons.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "VRCharacter/VRPlayer.h"
#include "Components/WidgetComponent.h"
#include "../Plugins/MultiplayerSessions/Source/MultiplayerSessions/Public/Menu.h"



// Sets default values
AButtons::AButtons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	ButtonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonComp"));
	ButtonComp->SetupAttachment(RootComponent);
	PushButtonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PushButtonComp"));
	PushButtonComp->SetupAttachment(RootComponent);

	JoinWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("JoinWidgetComp"));
	JoinWidgetComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FClassFinder<UMenu> MenuWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/MultiplayerSessions/WBP_Menu.WBP_Menu_C'"));
	if (MenuWidget.Succeeded())
	{
		JoinWidgetComp->SetWidgetClass(MenuWidget.Class);
		JoinWidgetComp->SetVisibility(false);
	}


	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AButtons::OnButtonOverlap);
}

// Called when the game starts or when spawned
void AButtons::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AButtons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPressed && Timer <= 1)
	{	
		Timer += DeltaTime;
		PushButtonComp->SetRelativeLocation(FVector(0, 0, EaseOutElasticRev(Timer / 3) * 20));	
	}
	else if (IsPressed && Timer >1)
	{
		IsPressed = false;
		Cast<UMenu>(JoinWidgetComp->GetWidget())->ClickJoinButton();
	}
	
}
void AButtons::OnButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AVRPlayer>(OtherActor);
	if (Player)
	{
		IsPressed = true;
	}

}

float AButtons::EaseOutElasticRev(float Number)
{
	const float Num = UKismetMathLibrary::MultiplyByPi(2) / 3;

	if (Number == 0)
	{
		return 0;
	}
	else if (Number == 1)
	{
		return -1;
	}
	else
	{
		return -(FMath::Pow(2, -10 * Number) * FMath::Sin((Number * 10 - 0.75) * Num) + 1);
	}
}

