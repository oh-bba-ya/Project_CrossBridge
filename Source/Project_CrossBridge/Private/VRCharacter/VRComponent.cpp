// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter/VRComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVRComponent::AVRComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetIsReplicated(true);

}

// Called when the game starts or when spawned
void AVRComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsVRHead)
	{
		UMaterialInterface* HeadBase = MeshComp->GetMaterial(0);
		if (HeadBase)
		{
			HeadMat = MeshComp->CreateDynamicMaterialInstance(0, HeadBase);
			HeadMat->SetVectorParameterValue(FName("HeadColor"), (FLinearColor)FVector(0.8, 0, 0));
		}
	}
	
}

// Called every frame
void AVRComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRComponent::MulticastHeadColorChange_Implementation(float Rate)
{
	if (Rate <= 0.5)
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(0.8, 0, 0), FVector(0.8, 0.8, 0), Rate * 2);
		HeadMat->SetVectorParameterValue(FName("HeadColor"), (FLinearColor)ColorVector);
	}
	else if (Rate <= 1)
	{
		FVector ColorVector = UKismetMathLibrary::VLerp(FVector(0.8, 0.8, 0), FVector(0, 0.8, 0), (Rate - 0.5) * 2);
		HeadMat->SetVectorParameterValue(FName("HeadColor"), (FLinearColor)ColorVector);
	}
}