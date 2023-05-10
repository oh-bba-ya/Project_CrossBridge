// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TrashCanProjectile.h"

#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Objects/VRCore.h"

// Sets default values
ATrashCanProjectile::ATrashCanProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionProfileName(FName("TrashBombPreset"));
	if(GetLocalRole() == ROLE_Authority)
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &ATrashCanProjectile::OnProjectileImpact);
	}

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));

	BoxComponent->SetSimulatePhysics(true);

	DamageType = UDamageType::StaticClass();
	damagePower = 10.f;
	
	
}

// Called when the game starts or when spawned
void ATrashCanProjectile::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ATrashCanProjectile::OnBeginOverlap);
	
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,FTimerDelegate::CreateLambda([this]()->void{Destroy();}),DestroyTime,false);
	
}

void ATrashCanProjectile::Destroyed()
{
	FVector SpawnLoc = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, SpawnLoc,FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	
}

void ATrashCanProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, damagePower, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
	}
	Destroyed();
}

// Called every frame
void ATrashCanProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrashCanProjectile::Fire(FVector targetLoc)
{
	FVector startLoc = GetActorLocation();
	FVector outVelocity = FVector::ZeroVector;

	if(UGameplayStatics::SuggestProjectileVelocity_CustomArc(this,outVelocity,startLoc,targetLoc,GetWorld()->GetGravityZ(),arcValue))
	{
		if(bDrawLine)
		{
			FPredictProjectilePathParams predictParams(20.0f,startLoc,outVelocity,15.f);
			predictParams.DrawDebugTime = 15.f;
			predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
			FPredictProjectilePathResult result;
			UGameplayStatics::PredictProjectilePath(this,predictParams,result);
		}
		BoxComponent->AddImpulse(outVelocity*BoxComponent->GetMass());
	}
}

void ATrashCanProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if(HasAuthority())
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);
		if(player != nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Not Null Player"));
			if(player->IsVR)
			{
				UE_LOG(LogTemp,Warning,TEXT("Player VR"));
				player->VRGetDamage(damagePower);
			}
		}

		AVRCore* core = Cast<AVRCore>(OtherActor);
		if(core != nullptr)
		{
			core->OnTakeDamage(damagePower);
			core->ChangeColor();
		}

		Destroy();
	}
}


