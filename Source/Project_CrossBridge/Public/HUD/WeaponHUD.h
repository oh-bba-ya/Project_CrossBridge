// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WeaponHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDStruct
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairCenter;
	class UTexture2D* CrosshairLeft;
	class UTexture2D* CrosshairRight;
	class UTexture2D* CrosshairTop;
	class UTexture2D* CrosshairBottom;
};


/**
 * 
 */


UCLASS()
class PROJECT_CROSSBRIDGE_API AWeaponHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	/** Character Widget HP */

	UPROPERTY(EditAnywhere, Category="Settings|Player Status")
	TSubclassOf<class UUserWidget> CharacterWidget;

	class UBaseCharacterWidget* CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();



private:
	FHUDStruct HUDStruct;

	void DrawCrossHair(UTexture2D* texture, FVector2D ViewportCenter);
public:
	FORCEINLINE void SetHUDStruct(const FHUDStruct& hud) {HUDStruct = hud;}
	
	
};
