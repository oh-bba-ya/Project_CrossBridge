// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/BaseCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionProfileName(FName("BulletPreset"));
	SphereComponent->SetSphereRadius(32.f);
	SphereComponent->SetWorldScale3D(FVector(0.2f,0.2f,0.2f));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(0.2f,0.2f,0.2f));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	// 이동방향으로 회전..
	MovementComponent->bRotationFollowsVelocity =true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	FTimerHandle destroyTimer;

	GetWorld()->GetTimerManager().SetTimer(destroyTimer,FTimerDelegate::CreateLambda([this]()->void
	{
		Destroy();
	}),LifeSpan,false);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ReadyShoot(DeltaTime);
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString s = OtherActor->GetName();
	UE_LOG(LogTemp,Warning,TEXT("Name : %s"),*s);
	if (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel8)
	{
		class ABaseCharacter* VRPlayer = Cast<ABaseCharacter>(OtherActor);
		if (VRPlayer && HasAuthority())
		{
			VRPlayer->VRGetDamage(5);
		}
	}
	Destroy();
	
}

void AProjectile::ReadyShoot(float deltaTime)
{
	if(Ready)
	{
		return ;
	}
	
	if(!Ready)
	{
		currentTime += deltaTime;
		CurrentScale = FMath::Lerp<float>(CurrentScale,ScaleLimit,currentTime/GrowTime);

		SphereComponent->SetWorldScale3D(FVector(CurrentScale,CurrentScale,CurrentScale));
	}


	if(currentTime >= GrowTime)
	{
		SphereComponent->SetWorldScale3D(FVector(ScaleLimit,ScaleLimit,ScaleLimit));
		Ready = true;
	}
	
}

