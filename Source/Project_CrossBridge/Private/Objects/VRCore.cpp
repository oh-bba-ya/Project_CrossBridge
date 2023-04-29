// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/VRCore.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AVRCore::AVRCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AVRCore::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetHealth(MaxHealth);
	}


	// 현재 색상 값 저장
	UMaterialInterface* iMat = MeshComponent->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("MyColor"));

	iMat->GetVectorParameterValue(param,initColor);

	dynamicMat = UMaterialInstanceDynamic::Create(iMat,this);

	if(dynamicMat != nullptr)
	{
		MeshComponent->SetMaterial(0,dynamicMat);
	}
	
}

// Called every frame
void AVRCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AVRCore::SetHealth(float v)
{
	v = v >= 0 ? v : 0;
	CureentHealth = FMath::Min(MaxHealth,v);
}

void AVRCore::AddHealth(float v)
{
	CureentHealth = FMath::Clamp(CureentHealth+v,0,MaxHealth);
}

void AVRCore::SubtractHealth(float v)
{
	CureentHealth = FMath::Clamp(CureentHealth-v,0,MaxHealth);
}

void AVRCore::OnTakeDamage(float v)
{
	Server_OnTakeDamage(v);
}


void AVRCore::Server_OnTakeDamage_Implementation(float v)
{
	SubtractHealth(v);
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, FString::Printf(TEXT("VRCore HP : %.1f"),CureentHealth));
}

void AVRCore::ChangeColor()
{
	linearColor = linearColor + FVector(0.1f,0.f,0.f);
	dynamicMat->SetVectorParameterValue(TEXT("MyColor"),linearColor);
}

// 서버에 복제 등록하기 위한 함수
void AVRCore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRCore, CureentHealth);
	DOREPLIFETIME(AVRCore, linearColor);

}

