// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "PCSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API APCSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	FORCEINLINE void SetOrigin(class ABaseCharacter* p) {originPC = p;}

private:
	UFUNCTION()
	void ChangePCPlayer();

	UPROPERTY(EditDefaultsOnly, Category="Settings|RespawnTime")
	int32 reSpawnTime = 15;


	UPROPERTY(EditDefaultsOnly, Category="Settings|PCPlayer")
	TSubclassOf<class ABaseCharacter> PCPawn;

	class ABaseCharacter* originPC;
	
	
};
