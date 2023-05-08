// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TrashCan.h"

#include "Character/BaseCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/TrashCanProjectile.h"

// Sets default values
ATrashCan::ATrashCan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionProfileName(FName("WeaponPreset"));
	BoxComponent->SetSimulatePhysics(true);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->SetRelativeRotation(FRotator(90.f,0.f,0.f));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));
	

}

// Called when the game starts or when spawned
void ATrashCan::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrashCan::OnBeginOverlap);
	
}

// Called every frame
void ATrashCan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ATrashCan::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(OtherActor);
		if(player != nullptr && player->GetTrashCan() == nullptr && !(player->IsVR))
		{
			 // 쓰레기통은 쓰레기통을 소지하지 못한 타플레이어가 뺏을 수 없음
			if(OwnerPlayer == nullptr)
			{
				UE_LOG(LogTemp,Warning,TEXT("PickUPTrashCan"));
				PickUpTrashCan(player);
			}
		}
	}
}

void ATrashCan::PickUpTrashCan(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		Server_PickUpTrashCan(player);
	}
}

void ATrashCan::DrawTrajectory()
{
	// LinePoins 초기화
	LinePoints.RemoveAt(0, LinePoints.Num());
	
	FVector Pos = GetActorLocation();
	FVector Dir = GetActorUpVector();

	LinePoints.Add(Pos);

	for(int i=0;i<LineSmooth;i++)
	{
		FVector LastPos = Pos;

		Dir += FVector::UpVector * Gravity * SimulateTime;

		Pos += Dir * SimulateTime;

		LinePoints.Add(Pos);
	}

	for(int i=0;i<LinePoints.Num()-1;i++)
	{
		DrawDebugLine(GetWorld(),LinePoints[i],LinePoints[i+1],FColor::Red,false,-1,0,1);
	}
}

void ATrashCan::Fire(ABaseCharacter* player,const FVector hitTarget)
{
	if(player != nullptr && Count >= FireCanCount)
	{
		Server_Fire(player, hitTarget);
	}
}

void ATrashCan::Server_Fire_Implementation(ABaseCharacter* player, const FVector hitTarget)
{
	if(projectileFactory)
	{
		Count -= FireCanCount;
		bFireDelay = false;
		FTimerHandle fireDelayHandle;
		GetWorldTimerManager().SetTimer(fireDelayHandle,FTimerDelegate::CreateLambda([&]{bFireDelay=true;}),FireDelayTime,false);
		
		ATrashCanProjectile* Projectile  = GetWorld()->SpawnActor<ATrashCanProjectile>(projectileFactory,GetActorLocation(),ArrowComponent->GetRelativeRotation());
		FVector ToTarget = GetActorLocation() - Projectile->GetActorLocation();
		ToTarget.GetSafeNormal();
		
		ToTarget = ToTarget*800.f;
		
		if(Projectile != nullptr)
		{
			Projectile->BoxComponent->SetPhysicsLinearVelocity(ToTarget);
			Projectile->SetOwner(player);
		}
	}
}

void ATrashCan::Multicast_PickUpTrashCan_Implementation(ABaseCharacter* player)
{
	if(player != nullptr)
	{
		player->SetTrashCan(this);
		BoxComponent->SetSimulatePhysics(false);
		AttachToComponent(player->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale, FName("TrashCanBag"));
		BoxComponent->SetCollisionProfileName(FName("NoCollision"));
		OwnerPlayer = player;
	}
}

void ATrashCan::Server_PickUpTrashCan_Implementation(ABaseCharacter* player)
{
	Multicast_PickUpTrashCan(player);
	SetOwner(player);
}


// 서버에 복제 등록하기 위한 함수
void ATrashCan::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATrashCan,Count);
	DOREPLIFETIME(ATrashCan,bFireDelay);
}

