// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Belica/CBelicaAnim.h"
#include "Character/CBelica.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCBelicaAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	belica = Cast<ACBelica>(GetOwningActor());
}

void UCBelicaAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(belica == nullptr)
	{
		return ;
	}

	moveSpeed = belica->GetVelocity().Length();

	rotAngle = UKismetAnimationLibrary::CalculateDirection(belica->GetVelocity(),belica->GetActorRotation());
	
	isinAir = belica->GetCharacterMovement()->IsFalling();
	
}
