// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseGrabbableActor.h"
#include "Character/BaseCharacter.h"
#include "EngineUtils.h"


// Sets default values
ABaseGrabbableActor::ABaseGrabbableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	SetReplicates(true);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetSimulatePhysics(true);

	OnActorBeginOverlap.AddDynamic(this, &ABaseGrabbableActor::OnGrabbableActorOverlap);
	
}

// Called when the game starts or when spawned
void ABaseGrabbableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGrabbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FindOwner();
	}
	FString myOwner = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	FString infoText = FString::Printf(TEXT("Owner: %s"),  *myOwner);


	if (IsThrow)
	{
		if (PCPlayer)
		{
			//PCPlayer->Server_TakeDamage(20);
		}
	}

}

void ABaseGrabbableActor::FindOwner()
{
	float MaxDist = 500;
	ABaseCharacter* Player = nullptr;

	for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	{
		float Dist = this->GetDistanceTo(*It);
		
		if (Dist < MaxDist)
		{
			MaxDist = Dist;
			Player = *It;
		}
	}

	if (Player && GetOwner() != Player)
	{
		SetOwner(Player);
	}
}

void ABaseGrabbableActor::OnGrabbableActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	PCPlayer = Cast<ABaseCharacter>(OtherActor);
}

void ABaseGrabbableActor::OnGrabbableActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	PCPlayer = NULL;
}