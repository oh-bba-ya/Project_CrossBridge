// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CityEnergyShield.h"

#include "Kismet/GameplayStatics.h"
#include "Objects/VRCore.h"

// Sets default values
ACityEnergyShield::ACityEnergyShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetWorldScale3D(FVector(30.f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if(tempMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(tempMesh.Object);
	}

}

// Called when the game starts or when spawned
void ACityEnergyShield::BeginPlay()
{
	Super::BeginPlay();

	core = Cast<AVRCore>(UGameplayStatics::GetActorOfClass(GetWorld(),AVRCore::StaticClass()));
	
	if(core != nullptr)
	{
		core->coreDelegate.BindDynamic(this, &ACityEnergyShield::DissolveShield);
	}
}

// Called every frame
void ACityEnergyShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACityEnergyShield::DissolveShield()
{
	if(Value > 1.6f)
	{
		return;
	}
	
	UMaterialInterface* IMat = MeshComponent->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("Dissolve"));

	IMat->GetScalarParameterValue(param,Value);
	//UE_LOG(LogTemp, Warning, TEXT("Value : %f"), Value);   // 현재 Dissolve의 값..



	// 현재 메쉬의 메테리얼 0번째를 생성..
	ShieldMat = ShieldMat == nullptr ? MeshComponent->CreateDynamicMaterialInstance(0,IMat) : ShieldMat;
	
	
	if(ShieldMat != nullptr)
	{
		// 생성된 메테리얼의 Dissovle 값을 CurDissolve로 변경..
		ShieldMat->SetScalarParameterValue(FName("Dissolve"),CurDissolve);
	}
	
	CurDissolve += AddDissolve;

	//UE_LOG(LogTemp, Warning, TEXT("TestFloat : %f"), CurDissolve);
	FTimerHandle dissolveHandle;
	
	GetWorld()->GetTimerManager().SetTimer(dissolveHandle,this,&ACityEnergyShield::DissolveShield,DissolveHadnleTime,false);
	

}


