// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECT_CROSSBRIDGE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasePlayer Camera Settings")
		class USpringArmComponent* springArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasePlayer Camera Settings")
		class UCameraComponent* camComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputMappingContext* BaseContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputAction* InputMovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input Settings")
		class UInputAction* InputJumpAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 하위 클래스에서 캐릭터마다 슈퍼점프, 일반점프, 제트팩 시스템을 구현할 예정이므로 virtual 키워드를 사용했다.
	virtual void Jump() override;    

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
