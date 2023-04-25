// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Character/BaseCharacter.h"

void UBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	baseCharacter = Cast<ABaseCharacter>(GetOwningActor());
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(baseCharacter != nullptr)
	{
		moveSpeed = baseCharacter->GetVelocity().Length();

		rotAngle = UKismetAnimationLibrary::CalculateDirection(baseCharacter->GetVelocity(),baseCharacter->GetActorRotation());

		isinAir = baseCharacter->GetCharacterMovement()->IsFalling();

		bIsFlying = baseCharacter->IsFlying();

		FRotator viewRot = baseCharacter->GetBaseAimRotation();
		FRotator baseRot = baseCharacter->GetActorRotation();
		FRotator deltaRot = baseRot-viewRot;

		pitch = FMath::Clamp(deltaRot.GetNormalized().Pitch,-45.f,45.f);
		
	}

}
