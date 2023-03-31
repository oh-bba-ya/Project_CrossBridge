// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CBelicaAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UCBelicaAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AnimSettings)
	float rotAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AnimSettings)
	float moveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AnimSettings)
	bool isinAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AnimSettings)
	bool bIsFlying = false;

private:
	class ACBelica* belica;
	
};
