// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/VRCore.h"

#include "CrossBridgeStateBase.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Objects/CoreTarget.h"


// Sets default values
AVRCore::AVRCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	laserComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserComponent"));
	laserComponent->SetupAttachment(MeshComponent);
	laserComponent->SetAutoActivate(false);

	VRCoreDestroyComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VRCoreDestroyComp"));
	VRCoreDestroyComp->SetupAttachment(RootComponent);
	VRCoreDestroyComp->SetAutoActivate(false);
	VRCoreDestroyComp->SetRelativeRotation(FRotator(0.f,0.f,90.f));

	ChargeComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VRCoreChargeComponent"));
	ChargeComponent->SetupAttachment(RootComponent);
	ChargeComponent->SetAutoActivate(true);

}

// Called when the game starts or when spawned
void AVRCore::BeginPlay()
{
	Super::BeginPlay();



	
	if(HasAuthority())
	{
		SetCurrentHealth(MaxHP);
	}

	BridgeState = Cast<ACrossBridgeStateBase>(GetWorld()->GetGameState());

	if(BridgeState != nullptr)
	{
		BridgeState->endStateDelegate.AddDynamic(this, &AVRCore::LaserFire);
		BridgeState->endStateDelegate.AddDynamic(this, &AVRCore::FallingVRCore);
	}

	Target = Cast<ACoreTarget>(UGameplayStatics::GetActorOfClass(GetWorld(),ACoreTarget::StaticClass()));
	
}

// Called every frame
void AVRCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

#pragma region Status Health
void AVRCore::SetCurrentHealth(float value)
{
	if(GetLocalRole() ==  ROLE_Authority)
	{
		CurrentHP = FMath::Clamp(value,0.f,MaxHP);
		OnHealthUpdate();
	}
}

void AVRCore::OnTakeDamage(float v)
{
	float hp = CurrentHP - v;
	SetCurrentHealth(hp);
}

void AVRCore::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}


void AVRCore::OnHealthUpdate()
{
	//클라이언트 전용 함수 기능
	if (!HasAuthority())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHP);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHP <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHP);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	//모든 머신에서 실행되는 함수 
	/*  
		여기에 대미지 또는 사망의 결과로 발생하는 특별 함수 기능 배치 
	*/

	if(CurrentHP <= 0)
	{
		if(BridgeState != nullptr)
		{
			BridgeState->GameWinner(EWinner::PC);
			BridgeState->GameMatchState(EGameState::End);
			ChargeComponent->SetActive(false);
		}
	}
}
#pragma endregion 


void AVRCore::LaserFire()
{
	if(bIsFire)
	{
		return;
	}

	if(BridgeState->GetWinner() != EWinner::VR)
	{
		return ;
	}

	ChargeComponent->SetActive(false);
	laserComponent->SetActive(true);
	bIsFire = true;

	FTimerHandle laserHandle;
	GetWorld()->GetTimerManager().SetTimer(laserHandle,this, &AVRCore::EndLaser,5.0f,false);
}

void AVRCore::EndLaser()
{
	laserComponent->SetActive(false);
	coreDelegate.ExecuteIfBound();
}

void AVRCore::FallingVRCore()
{
	if(BridgeState->GetWinner() != EWinner::PC)
	{
		return ;
	}

	
	


	if(Target != nullptr)
	{
		VRCoreDestroyComp->SetActive(true);
		float CurDisTance = (Target->GetActorLocation() - GetActorLocation()).Size();
		if(CurDisTance <= Distance)
		{
			return;
		}
		//UE_LOG(LogTemp,Warning,TEXT("Move"));
		FVector curLoc = GetActorLocation();

		FVector Direction = Target->GetActorLocation() - curLoc;

		FVector dir = Direction.GetSafeNormal();

		SetActorLocation(curLoc + dir * Speed * GetWorld()->GetDeltaSeconds());
		

		
	}

	
	FTimerHandle moveHandle;
	GetWorld()->GetTimerManager().SetTimer(moveHandle,this,&AVRCore::FallingVRCore,FallingCallTime,false);
	
}



// 서버에 복제 등록하기 위한 함수
void AVRCore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRCore, CurrentHP);

}

