// Fill out your copyright notice in the Description page of Project Settings.


#include "Spectator/PCSpectatorPawn.h"

#include "BaseCharacterController.h"
#include "Character/BaseCharacter.h"
#include "Project_CrossBridge/Project_CrossBridgeGameModeBase.h"

void APCSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle respawnHandle;
	GetWorldTimerManager().SetTimer(respawnHandle,this,&APCSpectatorPawn::ChangePCPlayer,reSpawnTime,false);
}

void APCSpectatorPawn::ChangePCPlayer()
{
	
	if(HasAuthority())
	{
		AProject_CrossBridgeGameModeBase* GM = Cast<AProject_CrossBridgeGameModeBase>(GetWorld()->GetAuthGameMode());

		if(GM != nullptr)
		{
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// 테스트 때문에 FVector(0,30,0) 더했음.. 지워야함..
			ABaseCharacter* PCPlayer = GetWorld()->SpawnActor<ABaseCharacter>(
				PCPawn,
				GM->GetDeadLocation() + FVector(0,0,50.f),
				GM->GetDeadRotation(),
				param);
			
			ABaseCharacterController* pc = Cast<ABaseCharacterController>(GetController());
			

			if(originPC != nullptr && pc != nullptr)
			{
				pc->Respawn(PCPlayer);
				pc->Possess(PCPlayer);
				Destroy();
			}
			
		}
	}
	
	
}
