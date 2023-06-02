// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

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

		isEquip = baseCharacter->GetIsEquip();

		bisDead = baseCharacter->GetPCPlayerDead();

		FRotator viewRot = baseCharacter->GetBaseAimRotation();
		FRotator baseRot = baseCharacter->GetActorRotation();
		FRotator deltaRot = baseRot-viewRot;

		pitch = FMath::Clamp(deltaRot.GetNormalized().Pitch,-45.f,45.f);
		
	}

}

void UBaseCharacterAnimInstance::AnimNotify_RollingActionRelease()
{
	baseCharacter->RollingActionReleased();
}

void UBaseCharacterAnimInstance::AnimNotify_SlidingActionRelease()
{
	baseCharacter->SlidingActionRelease();
}

void UBaseCharacterAnimInstance::AnimNotify_ChangeSpectator()
{
	baseCharacter->ChangeSpectator();
}

void UBaseCharacterAnimInstance::AnimNotify_WalkSound()
{
	if(baseCharacter != nullptr)
	{
		if(baseCharacter->IsLocallyControlled())
		{
			if(!(baseCharacter->IsVR))
			{
				if(WalkSounds && WalkAttenuation)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(),WalkSounds,baseCharacter->GetActorLocation(),1,1,0,WalkAttenuation);
				}
			}
		}
	}
}
