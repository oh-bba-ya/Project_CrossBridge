// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API ABaseCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	

public:
	UPROPERTY(EditAnywhere, Category="Settings|Widget")
	TSubclassOf<class UBaseCharacterWidget> baseCharacterWidget;

	FORCEINLINE UBaseCharacterWidget* GetBaseCharacterUI() {return baseCharacterUI;}

private:
	UBaseCharacterWidget* baseCharacterUI;
	
};
