// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_CROSSBRIDGE_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Anim")
	float rotAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Anim")
	float moveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings|Anim")
	bool isinAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings|Anim")
	bool bIsFlying = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings|Anim")
	bool bAiming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings|Anim")
	float pitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings|Anim")
	bool isEquip;
	
	UFUNCTION()
	void AnimNotify_RollingActionRelease();

	UFUNCTION()
	void AnimNotify_SlidingActionRelease();
	
private:
	class ABaseCharacter* baseCharacter;

	
	
	
};
